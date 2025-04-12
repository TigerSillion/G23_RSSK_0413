#ifndef DEBUG_UART_H
#define DEBUG_UART_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/* 配置：时间戳格式 */
typedef enum {
    DEBUG_TS_NONE = 0,     /* 无时间戳 */
    DEBUG_TS_MS,           /* 毫秒计数 */
    DEBUG_TS_RTC           /* RTC日期时间（需自行实现获取接口） */
} DebugTSFormat_t;

/* 配置：颜色 */
typedef enum {
    DEBUG_COLOR_NONE = 0,
    DEBUG_COLOR_RED,
    DEBUG_COLOR_GREEN,
    DEBUG_COLOR_YELLOW,
    DEBUG_COLOR_BLUE,
    DEBUG_COLOR_CYAN,
    DEBUG_COLOR_MAGENTA
} DebugColor_t;

/* 调试消息结构 */
typedef struct {
    uint32_t      timestamp;
    const char   *file;
    const char   *func;
    uint32_t      line;
    const char   *moduleID;
    DebugColor_t  color;
    char          message[64]; /* 实际内容（可调大小） */
} DebugMessage_t;

/* 库整体配置 */
typedef struct {
    DebugTSFormat_t tsFormat;
    uint16_t        minIntervalMs;      /* 最小处理间隔 */
    uint16_t        maxProcessTimeUs;   /* 单次最大处理时间(微秒) */
    uint16_t        bufferSize;         /* 环形缓冲大小(条数) */
    uint16_t        queueDepth;         /* 队列深度(环形队列) */
    bool            enableCRC;          /* 是否启用CRC校验 */
    bool            enableColor;        /* 是否启用彩色输出 */
} DebugConfig_t;

/**
 * @brief  初始化调试功能
 * @param  cfg: 配置指针
 * @note   需在UART外设初始化之后调用
 */
void Debug_Init(const DebugConfig_t *cfg);

/**
 * @brief  调用此函数记录并排队发送调试信息
 * @param  moduleID  : 模块ID
 * @param  color     : 输出颜色
 * @param  file      : 文件名(可使用__FILE__)
 * @param  func      : 函数名(可使用__func__ 或 __FUNCTION__)
 * @param  line      : 行号(可使用__LINE__)
 * @param  fmt, ...  : 格式化字符串与参数
 */
void Debug_Send(const char *moduleID,
                DebugColor_t color,
                const char *file,
                const char *func,
                uint32_t line,
                const char *fmt, ...);

/**
 * @brief  在主循环中调用，执行非阻塞发送
 *         每次调用建议限制在1ms内(或更短)的处理时间
 */
void Debug_Process(void);

/**
 * @brief  获取被丢弃的调试信息数量（队列满/超时等）
 */
uint32_t Debug_GetLostCount(void);

#ifdef __cplusplus
}
#endif

#endif /* DEBUG_UART_H */
