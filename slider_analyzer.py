#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
滑条性能分析工具 - 实时串口数据可视化
用于分析滑条的线性度和温度特性
"""

import argparse
import re
import sys
import threading
import time
from collections import deque
from datetime import datetime

import matplotlib.pyplot as plt
import numpy as np
import serial
import serial.tools.list_ports
from matplotlib.animation import FuncAnimation


class Config:
    """配置参数"""
    MAX_POINTS = 500  # 显示的最大数据点数
    UPDATE_INTERVAL = 200  # 更新间隔(毫秒)，增加以提高响应性
    BAUD_RATE = 115200  # 串口波特率
    COM_PORT = None  # 串口端口，如果为None则自动检测
    USE_ENGLISH = True  # 使用英文界面，避免字体问题
    RECENT_POINTS = 50  # 变化量图表显示的最近数据点数
    TEST_MODE = False  # 测试模式，不需要实际的串口连接


class Data:
    """数据存储"""
    def __init__(self):
        # 位置数据
        self.positions = deque(maxlen=Config.MAX_POINTS)
        self.position_changes = deque(maxlen=Config.MAX_POINTS)
        self.position_min = 999
        self.position_max = -999
        self.position_avg = 0
        self.position_sum = 0
        self.position_count = 0
        self.current_position = 0
        
        # 温度数据
        self.temperatures = deque(maxlen=Config.MAX_POINTS)
        self.temperature_changes = deque(maxlen=Config.MAX_POINTS)
        self.temp_min = 999
        self.temp_max = -999
        self.temp_avg = 0
        self.temp_sum = 0
        self.temp_count = 0
        self.current_temp = 0
        
        # 时间戳
        self.timestamps = deque(maxlen=Config.MAX_POINTS)
        self.start_time = time.time()
        
        # 统计数据
        self.position_avg_change = 0
        self.temp_avg_change = 0
        
        # 锁
        self.lock = threading.Lock()


def detect_com_ports():
    """检测可用的串口端口"""
    ports = list(serial.tools.list_ports.comports())
    if not ports:
        print("No serial devices detected. Please check your connection.")
        sys.exit(1)
        
    if len(ports) == 1:
        print(f"Automatically selected the only available port: {ports[0].device}")
        return ports[0].device
        
    print("Available serial ports:")
    for i, port in enumerate(ports):
        print(f"{i+1}. {port.device} - {port.description}")
        
    while True:
        try:
            choice = int(input("Select a port to use (enter number): "))
            if 1 <= choice <= len(ports):
                return ports[choice-1].device
        except ValueError:
            pass
        print("Invalid selection, please try again")


def parse_position_data(line, data, debug=False):
    """解析位置和温度数据行"""
    pos_match = re.search(r'Pos=(\d+)', line)
    temp_match = re.search(r'Temp=(\d+\.?\d*)', line)
    
    if pos_match and temp_match:
        # 提取位置和温度值
        position = int(pos_match.group(1))
        temperature = float(temp_match.group(1))
        
        with data.lock:
            # 更新位置数据
            data.positions.append(position)
            data.current_position = position
            data.position_sum += position
            data.position_count += 1
            data.position_avg = data.position_sum / data.position_count
            data.position_min = min(data.position_min, position)
            data.position_max = max(data.position_max, position)
            
            # 更新温度数据
            data.temperatures.append(temperature)
            data.current_temp = temperature
            data.temp_sum += temperature
            data.temp_count += 1
            data.temp_avg = data.temp_sum / data.temp_count
            data.temp_min = min(data.temp_min, temperature)
            data.temp_max = max(data.temp_max, temperature)
            
            # 更新时间戳
            data.timestamps.append(time.time() - data.start_time)
        
        if debug:
            print(f"Parsed data: Position={position}, Temperature={temperature}")
            
        return True
    return False


def parse_delta_data(line, data, debug=False):
    """解析变化量数据行"""
    pos_match = re.search(r'Pos=(-?\d+)', line)
    step_match = re.search(r'Step=(-?\d+\.?\d*)', line)
    
    if pos_match and step_match and "Delta:" in line:
        pos_change = int(pos_match.group(1))
        temp_change = float(step_match.group(1))
        
        with data.lock:
            # 更新变化量数据
            data.position_changes.append(pos_change)
            data.temperature_changes.append(temp_change)
            
            # 计算平均变化量 (忽略0值，仅计算绝对值的平均)
            non_zero_pos_changes = [abs(p) for p in data.position_changes if p != 0]
            non_zero_temp_changes = [abs(t) for t in data.temperature_changes if t != 0]
            
            if non_zero_pos_changes:
                data.position_avg_change = sum(non_zero_pos_changes) / len(non_zero_pos_changes)
            
            if non_zero_temp_changes:
                data.temp_avg_change = sum(non_zero_temp_changes) / len(non_zero_temp_changes)
        
        if debug:
            print(f"Parsed change: Position delta={pos_change}, Temperature step delta={temp_change}")
            
        return True
    return False


def calculate_changes(data):
    """计算并添加变化量数据（如果没有接收到变化量数据）"""
    with data.lock:
        if len(data.positions) < 2:
            return
            
        # 获取最新的位置和温度
        current_pos = data.positions[-1]
        current_temp = data.temperatures[-1]
        
        # 获取上一个位置和温度
        prev_pos = data.positions[-2]
        prev_temp = data.temperatures[-2]
        
        # 计算变化量
        pos_change = current_pos - prev_pos
        temp_change = current_temp - prev_temp
        
        # 只有当变化超过阈值时才记录
        if abs(pos_change) > 0 or abs(temp_change) > 0.05:
            data.position_changes.append(pos_change)
            data.temperature_changes.append(temp_change)
            
            # 更新平均变化量
            non_zero_pos_changes = [abs(p) for p in data.position_changes if p != 0]
            non_zero_temp_changes = [abs(t) for t in data.temperature_changes if abs(t) > 0.05]
            
            if non_zero_pos_changes:
                data.position_avg_change = sum(non_zero_pos_changes) / len(non_zero_pos_changes)
            
            if non_zero_temp_changes:
                data.temp_avg_change = sum(non_zero_temp_changes) / len(non_zero_temp_changes)


def serial_reader(ser, data, stop_event, debug=False):
    """串口读取线程"""
    buffer = ""
    last_calc_time = time.time()
    
    while not stop_event.is_set():
        try:
            if ser.in_waiting > 0:
                incoming = ser.read(ser.in_waiting).decode('utf-8', errors='ignore')
                buffer += incoming
                
                # 处理完整行
                lines = buffer.split('\n')
                buffer = lines.pop()  # 保留最后一个不完整的行
                
                data_updated = False
                for line in lines:
                    line = line.strip()
                    if not line:
                        continue
                        
                    if debug:
                        print(f"收到数据: {line}")
                    
                    # 尝试解析位置和温度数据
                    if parse_position_data(line, data, debug):
                        data_updated = True
                    else:
                        # 如果不是位置数据，尝试解析变化量数据
                        if parse_delta_data(line, data, debug):
                            data_updated = True
                
                # 如果接收到新的位置和温度数据，但没有变化量数据，则自动计算变化量
                current_time = time.time()
                if data_updated and (current_time - last_calc_time) > 0.5:
                    calculate_changes(data)
                    last_calc_time = current_time
            else:
                time.sleep(0.01)  # 避免CPU占用过高
                
        except Exception as e:
            print(f"串口读取错误: {e}")
            time.sleep(0.5)


def init_plot():
    """初始化图形界面"""
    plt.style.use('dark_background')
    fig = plt.figure(figsize=(12, 8))  # 增加图表宽度
    fig.canvas.manager.set_window_title("Slider Performance Analyzer")
    
    # 创建子图并调整间距
    gs = plt.GridSpec(4, 1, height_ratios=[1, 1, 0.8, 0.8], hspace=0.4)
    pos_ax = fig.add_subplot(gs[0])
    temp_ax = fig.add_subplot(gs[1])
    pos_delta_ax = fig.add_subplot(gs[2])
    temp_delta_ax = fig.add_subplot(gs[3])
    
    # 配置位置图
    pos_ax.set_ylabel("Position (0-100)")
    pos_ax.set_ylim(0, 100)
    pos_ax.grid(True, linestyle='--', alpha=0.7)
    pos_ax.set_title("Slider Position Waveform")
    pos_line, = pos_ax.plot([], [], 'g-', linewidth=1.5)
    
    # 配置温度图
    temp_ax.set_ylabel("Temperature (°C)")
    temp_ax.set_ylim(16, 31)
    temp_ax.grid(True, linestyle='--', alpha=0.7)
    temp_ax.set_title("Temperature Waveform")
    temp_line, = temp_ax.plot([], [], 'r-', linewidth=1.5)
    
    # 配置位置变化量图
    pos_delta_ax.set_ylabel("Position Change")
    pos_delta_ax.set_ylim(-10, 10)
    pos_delta_ax.grid(True, linestyle='--', alpha=0.7)
    pos_delta_ax.set_title("Position Step Change")
    pos_delta_bars = pos_delta_ax.bar([], [], width=0.8, color='cyan')
    
    # 配置温度变化量图
    temp_delta_ax.set_ylabel("Temperature Change")
    temp_delta_ax.set_xlabel("Sample Points")
    temp_delta_ax.set_ylim(-1, 1)
    temp_delta_ax.grid(True, linestyle='--', alpha=0.7)
    temp_delta_ax.set_title("Temperature Step Change")
    temp_delta_bars = temp_delta_ax.bar([], [], width=0.8, color='orange')
    
    # 添加文本区域显示统计数据
    stats_text = plt.figtext(0.02, 0.01, "", fontsize=9)
    
    # 调整布局
    plt.tight_layout(rect=[0, 0.03, 1, 0.98])
    
    return (fig, pos_line, temp_line, pos_delta_bars, temp_delta_bars, 
            pos_ax, temp_ax, pos_delta_ax, temp_delta_ax, stats_text)


def update_plot(frame, data, plot_elements):
    """更新图形"""
    pos_line, temp_line, pos_delta_bars, temp_delta_bars = plot_elements[:4]
    pos_ax, temp_ax, pos_delta_ax, temp_delta_ax, stats_text = plot_elements[4:]
    
    with data.lock:
        positions = list(data.positions)
        temperatures = list(data.temperatures)
        position_changes = list(data.position_changes)
        temperature_changes = list(data.temperature_changes)
    
    if positions:
        # 更新位置图
        x = range(len(positions))
        pos_line.set_data(x, positions)
        pos_ax.set_xlim(max(0, len(positions)-Config.MAX_POINTS), len(positions))
        
        # 更新温度图
        temp_line.set_data(x, temperatures)
        temp_ax.set_xlim(max(0, len(temperatures)-Config.MAX_POINTS), len(temperatures))
        if temperatures:
            temp_min = min(temperatures)
            temp_max = max(temperatures)
            if temp_max > temp_min:
                temp_ax.set_ylim(max(16, temp_min - 0.5), min(31, temp_max + 0.5))
    
    # 更新位置变化量图
    if position_changes:
        # 仅显示最近的数据点
        recent_pos_changes = position_changes[-Config.RECENT_POINTS:]
        x = list(range(len(recent_pos_changes)))  # 显式转换为列表
        
        # 完全清除旧数据并重绘
        pos_delta_ax.clear()
        pos_delta_ax.set_title("Position Step Change")
        pos_delta_ax.set_ylabel("Position Change")
        pos_delta_ax.set_ylim(-10, 10)
        pos_delta_ax.grid(True, linestyle='--', alpha=0.7)
        
        # 使用x刻度标签而不是设置xlim
        colors = ['cyan' if val >= 0 else 'blue' for val in recent_pos_changes]
        pos_delta_ax.bar(x, recent_pos_changes, width=0.8, color=colors)
        pos_delta_ax.set_xlim(-1, len(recent_pos_changes))
        
        # 强制使用整数刻度标签
        pos_delta_ax.set_xticks(x[::max(1, len(x)//5)])  # 设置适当数量的刻度
    
    # 更新温度变化量图
    if temperature_changes:
        # 仅显示最近的数据点
        recent_temp_changes = temperature_changes[-Config.RECENT_POINTS:]
        x = list(range(len(recent_temp_changes)))  # 显式转换为列表
        
        # 完全清除旧数据并重绘
        temp_delta_ax.clear()
        temp_delta_ax.set_title("Temperature Step Change")
        temp_delta_ax.set_ylabel("Temperature Change")
        temp_delta_ax.set_xlabel("Sample Points")
        temp_delta_ax.set_ylim(-1, 1)
        temp_delta_ax.grid(True, linestyle='--', alpha=0.7)
        
        # 使用x刻度标签而不是设置xlim
        colors = ['orange' if val >= 0 else 'yellow' for val in recent_temp_changes]
        temp_delta_ax.bar(x, recent_temp_changes, width=0.8, color=colors)
        temp_delta_ax.set_xlim(-1, len(recent_temp_changes))
        
        # 强制使用整数刻度标签
        temp_delta_ax.set_xticks(x[::max(1, len(x)//5)])  # 设置适当数量的刻度
    
    # 更新统计信息
    with data.lock:
        stats = (
            f"Position Stats: Min={data.position_min:.1f} Max={data.position_max:.1f} "
            f"Avg={data.position_avg:.1f} | "
            f"Temp Stats: Min={data.temp_min:.1f}°C Max={data.temp_max:.1f}°C "
            f"Avg={data.temp_avg:.1f}°C\n"
            f"Linearity Analysis: Pos Avg Step={data.position_avg_change:.2f} "
            f"Temp Avg Step={data.temp_avg_change:.2f} | "
            f"Current: Pos={data.current_position} Temp={data.current_temp:.1f}°C"
        )
        stats_text.set_text(stats)
    
    # 对于非blitting模式，不需要返回artists列表
    return None


def generate_test_data(data, stop_event):
    """生成测试数据"""
    import math
    import random
    
    sample_count = 0
    test_start_time = time.time()
    
    while not stop_event.is_set():
        # 生成一个模拟的正弦波形为位置
        t = (time.time() - test_start_time) * 0.5
        position = int(50 + 50 * math.sin(t))
        
        # 温度与位置相关，但有一定的随机性和相位差
        temperature = 22 + 5 * math.sin(t - 0.5) + random.uniform(-0.2, 0.2)
        
        with data.lock:
            # 更新位置数据
            if len(data.positions) > 0:
                # 计算变化量
                prev_pos = data.positions[-1]
                prev_temp = data.temperatures[-1]
                pos_change = position - prev_pos
                temp_change = temperature - prev_temp
                
                # 添加变化量（只有当变化超过阈值时）
                if abs(pos_change) > 0 or abs(temp_change) > 0.1:
                    data.position_changes.append(pos_change)
                    data.temperature_changes.append(temp_change)
                    
                    # 更新平均变化量
                    non_zero_pos_changes = [abs(p) for p in data.position_changes if p != 0]
                    non_zero_temp_changes = [abs(t) for t in data.temperature_changes if abs(t) > 0.05]
                    
                    if non_zero_pos_changes:
                        data.position_avg_change = sum(non_zero_pos_changes) / len(non_zero_pos_changes)
                    
                    if non_zero_temp_changes:
                        data.temp_avg_change = sum(non_zero_temp_changes) / len(non_zero_temp_changes)
            
            # 更新位置和温度数据
            data.positions.append(position)
            data.temperatures.append(temperature)
            data.current_position = position
            data.current_temp = temperature
            
            # 更新统计数据
            sample_count += 1
            data.position_sum += position
            data.position_count += 1
            data.position_avg = data.position_sum / data.position_count
            data.position_min = min(data.position_min, position)
            data.position_max = max(data.position_max, position)
            
            data.temp_sum += temperature
            data.temp_count += 1
            data.temp_avg = data.temp_sum / data.temp_count
            data.temp_min = min(data.temp_min, temperature)
            data.temp_max = max(data.temp_max, temperature)
            
            # 更新时间戳
            data.timestamps.append(time.time() - data.start_time)
            
        # 休眠一段时间以模拟实际的数据采样率
        time.sleep(0.1)


def main():
    """主函数"""
    parser = argparse.ArgumentParser(description="Slider Performance Analyzer - Real-time Serial Data Visualization")
    parser.add_argument("-p", "--port", help="Serial port (e.g. COM3 or /dev/ttyUSB0)")
    parser.add_argument("-b", "--baud", type=int, default=Config.BAUD_RATE, help="Baud rate")
    parser.add_argument("-d", "--debug", action="store_true", help="Enable debug mode")
    parser.add_argument("-t", "--test", action="store_true", help="Test mode (no serial connection required)")
    args = parser.parse_args()
    
    # 设置测试模式
    Config.TEST_MODE = args.test
    
    # 设置串口端口
    port = args.port if args.port else Config.COM_PORT
    if not port and not Config.TEST_MODE:
        port = detect_com_ports()
    
    # 初始化数据存储
    data = Data()
    
    # 初始化停止事件
    stop_event = threading.Event()
    
    try:
        # 初始化数据源线程
        if Config.TEST_MODE:
            print("Running in test mode - generating synthetic data")
            reader_thread = threading.Thread(
                target=generate_test_data,
                args=(data, stop_event),
                daemon=True
            )
            reader_thread.start()
        else:
            # 打开串口
            ser = serial.Serial(port, args.baud, timeout=1)
            print(f"Successfully connected to {port} (baud rate: {args.baud})")
            
            # 启动串口读取线程
            reader_thread = threading.Thread(
                target=serial_reader,
                args=(ser, data, stop_event, args.debug),
                daemon=True
            )
            reader_thread.start()
        
        # 初始化图形界面
        plot_elements = init_plot()
        fig = plot_elements[0]
        
        # 配置动画
        ani = FuncAnimation(
            fig, update_plot, 
            fargs=(data, plot_elements[1:]), 
            interval=Config.UPDATE_INTERVAL,
            blit=False,  # 禁用blitting以避免NoneType错误
            cache_frame_data=False
        )
        
        # 启用交互模式并减少GUI刷新频率
        plt.ion()
        plt.show()  # 确保图形显示窗口
        
        # 主循环
        try:
            while True:
                plt.pause(0.2)  # 增加暂停时间，减少CPU占用
        except KeyboardInterrupt:
            print("\nProgram stopped by user")
            
    except serial.SerialException as e:
        print(f"Serial error: {e}")
        print("Please check your serial connection")
    except Exception as e:
        print(f"Error: {e}")
        import traceback
        traceback.print_exc()  # 打印完整的错误信息
    finally:
        # 停止串口读取线程
        stop_event.set()
        try:
            if not Config.TEST_MODE and 'ser' in locals() and ser.is_open:
                ser.close()
                print("Serial port closed")
        except:
            pass
        plt.close('all')


if __name__ == "__main__":
    print("Slider Performance Analyzer - Real-time Serial Data Visualization")
    print("Press Ctrl+C to stop the program")
    main() 