import re
import time
import sys
import numpy as np

try:
    import serial
    from serial.tools import list_ports
except ImportError:
    print("Please install pyserial: pip install pyserial")
    sys.exit(1)

# Regular expression to extract the Pos value
LINE_REGEX = re.compile(r'Pos=(\d+)')

def find_port(baudrate=115200, timeout=1, test_duration=5):
    """
    Scan available serial ports and return the first one that outputs lines matching LINE_REGEX.
    
    Args:
        baudrate: Serial port baud rate
        timeout: Serial port timeout in seconds
        test_duration: Duration to test each port in seconds
        
    Returns:
        str: Port name if found, None if not found
    """
    print("Scanning for available serial ports...")
    ports = list_ports.comports()
    
    if not ports:
        print("No serial ports found on the system!")
        return None
        
    print(f"Found {len(ports)} serial port(s):")
    for port_info in ports:
        print(f"  - {port_info.device}: {port_info.description}")
    
    for port_info in ports:
        port = port_info.device
        print(f"\nTesting port {port}...")
        try:
            ser = serial.Serial(port, baudrate, timeout=timeout)
            print(f"  Successfully opened port {port}")
            print(f"  Waiting for data (timeout: {test_duration}s)...")
            
            start = time.time()
            while time.time() - start < test_duration:
                line = ser.readline().decode(errors='ignore')
                if line.strip():
                    print(f"  Received: {line.strip()}")
                if LINE_REGEX.search(line):
                    print(f"  Found matching data on port {port}!")
                    ser.close()
                    return port
                    
            print(f"  No matching data found on port {port}")
            ser.close()
        except (serial.SerialException, OSError) as e:
            print(f"  Error testing port {port}: {str(e)}")
            continue
            
    print("\nNo valid serial port found that matches the expected data format.")
    return None

def analyze_sweep(positions):
    """
    Analyze a single sweep of positions.
    Returns slope, intercept, R^2, maximum deviation, and a comprehensive score.
    """
    n = len(positions)
    x = np.arange(n)
    y = np.array(positions)
    a, b = np.polyfit(x, y, 1)
    y_fit = a * x + b
    
    # Calculate R²
    ss_res = np.sum((y - y_fit) ** 2)
    ss_tot = np.sum((y - np.mean(y)) ** 2)
    r2 = 1 - ss_res / ss_tot if ss_tot != 0 else float('nan')
    
    # Calculate maximum deviation
    max_dev = np.max(np.abs(y - y_fit))
    
    # Calculate comprehensive score (0-100)
    r2_score = min(60, r2 * 60) if not np.isnan(r2) else 0
    max_dev_score = max(0, 40 * (1 - max_dev / (np.max(y) - np.min(y))))
    total_score = r2_score + max_dev_score
    
    # Grade the performance
    if total_score >= 90:
        grade = "A (优秀)"
    elif total_score >= 80:
        grade = "B (良好)"
    elif total_score >= 70:
        grade = "C (一般)"
    elif total_score >= 60:
        grade = "D (及格)"
    else:
        grade = "F (不及格)"
    
    return a, b, r2, max_dev, total_score, grade

class SliderAnalyzer:
    def __init__(self):
        self.up_sweeps = []
        self.down_sweeps = []
        self.total_sweeps = 0
        
    def add_sweep(self, direction, score):
        if direction == 'up':
            self.up_sweeps.append(score)
        else:
            self.down_sweeps.append(score)
        self.total_sweeps += 1
        
    def get_summary(self):
        if self.total_sweeps == 0:
            return "暂无扫描数据"
            
        try:
            up_avg = np.mean(self.up_sweeps) if self.up_sweeps else 0
            down_avg = np.mean(self.down_sweeps) if self.down_sweeps else 0
            total_avg = (up_avg * len(self.up_sweeps) + down_avg * len(self.down_sweeps)) / self.total_sweeps if self.total_sweeps > 0 else 0
            
            def get_grade(score):
                if score >= 90: return "A (优秀)"
                if score >= 80: return "B (良好)"
                if score >= 70: return "C (一般)"
                if score >= 60: return "D (及格)"
                return "F (不及格)"
                
            return f"""
综合统计报告:
----------------
总扫描次数: {self.total_sweeps}
向上扫描次数: {len(self.up_sweeps)}
向下扫描次数: {len(self.down_sweeps)}

向上扫描平均分: {up_avg:.1f}/100 ({get_grade(up_avg)})
向下扫描平均分: {down_avg:.1f}/100 ({get_grade(down_avg)})
综合平均分: {total_avg:.1f}/100 ({get_grade(total_avg)})
----------------
"""
        except Exception as e:
            return f"生成统计报告时出错: {str(e)}"

def main(baudrate=115200):
    port = find_port(baudrate)
    if not port:
        print("No valid serial port found.")
        return

    print(f"Using port: {port} at {baudrate} baud.")
    ser = serial.Serial(port, baudrate, timeout=1)
    positions = []
    direction = None  # 'up' or 'down'
    analyzer = SliderAnalyzer()
    
    try:
        while True:
            line = ser.readline().decode(errors='ignore')
            m = LINE_REGEX.search(line)
            if not m:
                continue
            pos = int(m.group(1))
            if not positions:
                positions.append(pos)
                continue
            last = positions[-1]
            new_dir = 'up' if pos > last else 'down' if pos < last else direction
            if direction and new_dir != direction:
                a, b, r2, max_dev, score, grade = analyze_sweep(positions)
                print(f"\nSweep {direction.upper()} 分析结果:")
                print(f"斜率: {a:.2f}")
                print(f"截距: {b:.2f}")
                print(f"R²值: {r2:.4f}")
                print(f"最大偏差: {max_dev:.2f}")
                print(f"综合评分: {score:.1f}/100")
                print(f"等级: {grade}")
                
                analyzer.add_sweep(direction, score)
                print(analyzer.get_summary())
                
                positions = [last]
            positions.append(pos)
            direction = new_dir
    except KeyboardInterrupt:
        print("\n分析结束。")
        print(analyzer.get_summary())
    finally:
        ser.close()

if __name__ == '__main__':
    main()

