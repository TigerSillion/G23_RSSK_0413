#include "debug_uart.h"
#include <stdarg.h>   /* for va_list */
#include <stdio.h>    /* for vsnprintf */
#include <string.h>   /* for memset */
#include <stddef.h>   /* for NULL */
#include "r_smc_entry.h"
#include "../smc_gen/Config_UARTA1/Config_UARTA1.h"
#include "../smc_gen/general/r_cg_uarta.h"

/* ---------------- 用户需实现或对接的底层函数 ---------------- */
extern uint16_t g_uarta1_tx_count;
/* 全局毫秒计数器 */
extern volatile uint32_t g_msCounter;

//#define UART_SEND(data, length) R_Config_UARTA1_Send((uint8_t *)(data), length)
#define UART_SEND(data, length) NOP()

/**
 * @brief  获取当前毫秒计数
 */
static uint32_t GetCurrentMs(void)
{
    return g_msCounter;
}

/**
 * @brief  获取当前RTC时间(若需要日期时间组合)
 */
static uint32_t GetCurrentRTC(void)
{
    return 0;
}

/**
 * @brief  计算并返回简单的CRC值
 */
static uint8_t CalculateCRC(const uint8_t *data, uint16_t len)
{
    uint8_t crc = 0;
    for(uint16_t i = 0; i < len; i++)
        crc ^= data[i];
    return crc;
}

/**
 * @brief  UART发送函数(非阻塞)
 */
static void UART_SendNonBlocking(const uint8_t *data, uint16_t length)
{
    UART_SEND(data, length);
}

/**
 * @brief  判断当前UART是否空闲
 */
static bool HW_UART_Idle(void)
{
    // 简化UART状态检查，只检查发送是否完成
    return (g_uarta1_tx_count == 0U);
}

/* 调试状态机状态 */
typedef enum {
    DEBUG_STATE_IDLE,     // 空闲状态
    DEBUG_STATE_SENDING   // 正在发送状态
} DebugState_t;

/* -------------------------- 内部变量 -------------------------- */

static DebugConfig_t    g_debugCfg;
static DebugMessage_t  *g_messageBuf   = NULL;
static uint32_t         g_lostCount    = 0;    /* 丢失计数 */
static volatile uint16_t g_writeIndex = 0;
static volatile uint16_t g_readIndex  = 0;
static volatile uint16_t g_msgCount   = 0;    /* 队列中未发送的消息数 */
static uint32_t         g_lastProcessTimeMs = 0;   /* 用于最小间隔判断 */
static DebugState_t     g_debugState = DEBUG_STATE_IDLE;  /* 当前状态 */

/* 外部发送缓存 */
static char g_txBuffer[128];  /* 用于最终拼接输出 */

/* -------------------------- 内部函数 -------------------------- */

/**
 * @brief  获取时间戳
 */
static uint32_t GetTimestamp(void)
{
    switch(g_debugCfg.tsFormat)
    {
    case DEBUG_TS_MS:
        return GetCurrentMs();
    case DEBUG_TS_RTC:
        return GetCurrentRTC();
    default:
        return 0;
    }
}

/**
 * @brief  输出彩色前缀
 */
static const char* GetColorCode(DebugColor_t color)
{
    if(!g_debugCfg.enableColor) return "";
    switch(color) {
    case DEBUG_COLOR_RED:     return "\033[31m";
    case DEBUG_COLOR_GREEN:   return "\033[32m";
    case DEBUG_COLOR_YELLOW:  return "\033[33m";
    case DEBUG_COLOR_BLUE:    return "\033[34m";
    case DEBUG_COLOR_CYAN:    return "\033[36m";
    case DEBUG_COLOR_MAGENTA: return "\033[35m";
    default:                  return "\033[0m";
    }
}

/**
 * @brief  输出彩色后缀(还原)
 */
static const char* GetColorReset(void)
{
    return g_debugCfg.enableColor ? "\033[0m" : "";
}

/* -------------------------- API实现 -------------------------- */

void Debug_Init(const DebugConfig_t *cfg)
{
    if(cfg == NULL) return;

    /* 保存配置 */
    memcpy(&g_debugCfg, cfg, sizeof(DebugConfig_t));

    /* 初始化缓冲区 */
    static DebugMessage_t staticBuffer[256];
    g_messageBuf = staticBuffer;

    g_writeIndex = 0;
    g_readIndex  = 0;
    g_msgCount   = 0;
    g_lostCount  = 0;

    /* 根据实际需要，初始化UART硬件 */
    /* ... */
}

void Debug_Send(const char *moduleID,
                DebugColor_t color,
                const char *file,
                const char *func,
                uint32_t line,
                const char *fmt, ...)
{
    if(!g_messageBuf || !moduleID || !file || !func || !fmt) return;

    /* 环形队列是否已满 */
    if(g_msgCount >= g_debugCfg.bufferSize) {
        g_lostCount++;
        return;
    }

    /* 准备写入队列 */
    DebugMessage_t *pMsg = &g_messageBuf[g_writeIndex];

    pMsg->timestamp = GetTimestamp();
    pMsg->file      = file;
    pMsg->func      = func;
    pMsg->line      = line;
    pMsg->moduleID  = moduleID;
    pMsg->color     = color;

    /* 格式化写入message */
    va_list args;
    va_start(args, fmt);
    vsnprintf(pMsg->message, sizeof(pMsg->message), fmt, args);
    va_end(args);

    /* 更新写指针 */
    g_writeIndex = (g_writeIndex + 1) % g_debugCfg.bufferSize;
    /* 增加计数 */
    g_msgCount++;
}

void Debug_Process(void)
{
    uint32_t nowMs = GetCurrentMs();
    
    /* 状态机处理 */
    switch(g_debugState) {
    case DEBUG_STATE_IDLE:
        /* 检查是否有消息需要发送 */
        if(g_msgCount > 0) {
            /* 检查间隔时间 */
            if((nowMs - g_lastProcessTimeMs) >= g_debugCfg.minIntervalMs) {
                g_lastProcessTimeMs = nowMs;
                
                /* 取出消息 */
                DebugMessage_t *pMsg = &g_messageBuf[g_readIndex];
                
                /* 拼接待发送字符串 */
                int len = 0;
                len += snprintf(g_txBuffer + len, sizeof(g_txBuffer) - len, "%s", GetColorCode(pMsg->color));
                
                if(g_debugCfg.tsFormat != DEBUG_TS_NONE) {
                    len += snprintf(g_txBuffer + len, sizeof(g_txBuffer) - len,
                                   "[Time=%lu] ", (unsigned long)pMsg->timestamp);
                }
                len += snprintf(g_txBuffer + len, sizeof(g_txBuffer) - len,
                               "(%s:%lu, %s) [%s] %s%s\r\n",
                               pMsg->file,
                               (unsigned long)pMsg->line,
                               pMsg->func,
                               pMsg->moduleID,
                               pMsg->message,
                               GetColorReset());
                
                /* 确保最后有\r\n结尾 */
                if (len > 2 && 
                    !(g_txBuffer[len-2] == '\r' && g_txBuffer[len-1] == '\n')) {
                    len += snprintf(g_txBuffer + len, sizeof(g_txBuffer) - len, "\r\n");
                }
                
                /* 若启用CRC，在末尾追加CRC值 */
                if(g_debugCfg.enableCRC) {
                    uint8_t crc = CalculateCRC((uint8_t *)g_txBuffer, (uint16_t)len);
                    if((len + 6) < (int)sizeof(g_txBuffer)) {
                        len += snprintf(g_txBuffer + len, sizeof(g_txBuffer) - len, "[CRC=%02X]\r\n", crc);
                    }
                }
                
                /* 开始发送 */
                UART_SendNonBlocking((const uint8_t *)g_txBuffer, (uint16_t)len);
                
                /* 进入发送状态 */
                g_debugState = DEBUG_STATE_SENDING;
            }
        }
        break;
        
    case DEBUG_STATE_SENDING:
        /* 检查UART是否空闲 */
        if(HW_UART_Idle()) {
            /* 消息发送完成，更新读指针 */
            g_readIndex = (g_readIndex + 1) % g_debugCfg.bufferSize;
            g_msgCount--;
            
            /* 返回空闲状态 */
            g_debugState = DEBUG_STATE_IDLE;
            
            /* 立即尝试处理下一条消息 */
            if(g_msgCount > 0) {
                Debug_Process();
            }
        }
        break;
    }
}

uint32_t Debug_GetLostCount(void)
{
    return g_lostCount;
}
