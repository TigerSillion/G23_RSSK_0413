# 滑条性能分析工具

本工具用于实时读取串口数据并可视化显示滑条位置和温度波形，帮助分析滑条的线性度和温度特性。

## 功能特点

- 实时显示滑条位置波形（0-100）
- 实时显示温度数据波形（16-31°C）
- 显示位置和温度的变化量
- 自动统计数据：最小值、最大值、平均值
- 分析滑条线性度：位置平均步进、温度平均步进
- 支持自动检测串口或手动指定串口
- 调试模式可查看原始数据

## 安装依赖

在使用本工具前，需要安装以下Python依赖：

```bash
pip install pyserial matplotlib numpy
```

## 使用方法

### 基本用法

```bash
python slider_analyzer.py
```

程序启动后会自动检测系统上的可用串口，如果只有一个串口设备，会自动连接；如果有多个设备，会提示用户选择。

### 命令行参数

可以通过命令行参数指定串口和波特率：

```bash
python slider_analyzer.py -p COM3 -b 115200
```

参数说明：
- `-p, --port`：指定串口端口，如 COM3、/dev/ttyUSB0 等
- `-b, --baud`：指定波特率，默认为 115200
- `-d, --debug`：启用调试模式，输出详细日志

## 数据格式要求

本工具期望从串口接收以下格式的数据：

1. 位置和温度数据：
   ```
   Pos=50 Temp=25.5 ms=1234
   ```
   其中 `Pos` 是滑条位置（0-100），`Temp` 是温度值，`ms` 是时间戳。

2. 差异数据：
   ```
   Delta: Pos=-2 Step=0.5 ms=1235
   ```
   其中 `Pos` 是位置变化量，`Step` 是温度步数变化。

## 程序界面

程序界面分为四个部分：
1. 上方：滑条位置波形图（0-100）
2. 中上：温度值波形图（16-31°C）
3. 中下：位置变化量柱状图
4. 下方：温度步数变化柱状图

底部显示统计信息：
- 位置统计：最小值、最大值、平均值
- 温度统计：最小值、最大值、平均值
- 线性度分析：位置平均步进、温度平均步进
- 当前值：位置、温度

## 调试串口数据

要在固件中输出符合格式的数据，可参考以下示例：

```c
// 在主循环中定期输出位置和温度数据
void report_slider_data(void) {
    static uint32_t last_pos = 0;
    static float last_temp = 0;
    
    uint32_t current_pos = get_slider_position();  // 获取当前滑条位置 (0-100)
    float current_temp = get_temperature();        // 获取当前温度值
    uint32_t ms = get_milliseconds();              // 获取系统时间戳
    
    // 输出当前位置和温度
    printf("Pos=%d Temp=%.1f ms=%d\r\n", current_pos, current_temp, ms);
    
    // 计算并输出变化量
    int32_t delta_pos = current_pos - last_pos;
    float delta_temp = current_temp - last_temp;
    printf("Delta: Pos=%d Step=%.1f ms=%d\r\n", delta_pos, delta_temp, ms);
    
    // 更新上次值
    last_pos = current_pos;
    last_temp = current_temp;
}
```

## 数据分析技巧

使用本工具分析滑条性能时，请注意：

1. **线性度分析**：理想情况下，平均步进应该尽量恒定，位置变化量的柱状图应该高度一致
2. **温度影响**：观察温度变化时滑条位置是否有明显漂移
3. **响应特性**：快速滑动手指时，观察波形的响应速度和稳定性
4. **噪声分析**：静止不动的滑条，位置值应该保持稳定，噪声小

## 故障排除

1. **无法检测到串口**
   - 检查设备是否正确连接
   - 检查是否安装了设备驱动
   - 检查是否有其他程序占用了串口

2. **数据不显示**
   - 确认串口波特率设置正确
   - 检查串口数据格式是否符合要求
   - 使用 `-d` 参数启用调试模式，查看接收的原始数据

3. **程序崩溃**
   - 检查Python版本（推荐使用Python 3.6+）
   - 更新依赖包到最新版本
   
4. **中文显示问题**
   - 如果遇到"Glyph missing from font(s)"警告，说明缺少中文字体
   - 最新版本程序默认使用英文界面，避免此问题
   - 如需中文界面，可以尝试安装中文字体：
     - Windows：安装微软雅黑等中文字体
     - Linux：`sudo apt-get install fonts-wqy-microhei`
     - macOS：安装华文黑体等中文字体
   - 或修改 `Config.USE_ENGLISH = False` 切换回中文界面

5. **动画更新错误**
   - 如遇到 "AttributeError: 'NoneType' object has no attribute '_get_view'" 错误
   - 程序已将 `blit=True` 改为 `blit=False` 解决此问题
   - 如仍有问题，可以尝试降低更新频率（修改 `UPDATE_INTERVAL` 为更大的值） 