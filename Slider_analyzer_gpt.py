import pandas as pd
import numpy as np
from sklearn.linear_model import LinearRegression
import matplotlib.pyplot as plt
import os

def load_and_validate_data(file_path):
    if not os.path.exists(file_path):
        raise FileNotFoundError(f"找不到文件: {file_path}")
    
    df = pd.read_csv(file_path)
    required_columns = ["config01:Slider00:Status", "TS01:Measurement Value"]
    
    if not all(col in df.columns for col in required_columns):
        raise ValueError(f"CSV文件缺少必要的列: {required_columns}")
    
    return df

def analyze_slider_data(df):
    # 仅保留有效采样
    valid = df[df["config01:Slider00:Status"] == 0].copy()
    if len(valid) == 0:
        raise ValueError("没有找到有效的采样数据")
    
    valid["t"] = np.arange(len(valid))*0.02    # 20 ms / 点

    # 识别上升段与下降段 (根据斜率符号)
    grad = np.gradient(valid["TS01:Measurement Value"])
    peak = grad.argmax()                       # 正峰值后反向视为拐点
    up   = valid.iloc[:peak]
    down = valid.iloc[peak:]

    return valid, up, down, grad

def fit_line(seg):
    if len(seg) < 2:
        raise ValueError("数据点太少，无法进行线性回归")
    
    mdl = LinearRegression().fit(seg[["t"]], seg["TS01:Measurement Value"])
    pred = mdl.predict(seg[["t"]])
    err  = np.abs(seg["TS01:Measurement Value"]-pred)
    return mdl.coef_[0], mdl.intercept_, err.max(), err.mean(), pred

def plot_results(valid, up, down, up_pred, down_pred):
    plt.figure(figsize=(12, 6))
    plt.plot(valid["t"], valid["TS01:Measurement Value"], 'b.', label='原始数据')
    plt.plot(up["t"], up_pred, 'r-', label='上升段拟合')
    plt.plot(down["t"], down_pred, 'g-', label='下降段拟合')
    plt.xlabel('时间 (s)')
    plt.ylabel('测量值')
    plt.title('滑块数据分析')
    plt.legend()
    plt.grid(True)
    plt.savefig('slider_analysis.png')
    plt.close()

def main():
    try:
        # 加载数据
        file_path = "2883_20ms_65S_Slider_250424.csv"
        df = load_and_validate_data(file_path)
        
        # 分析数据
        valid, up, down, grad = analyze_slider_data(df)
        
        # 进行线性回归
        ku, bu, max_err_u, mae_u, up_pred = fit_line(up)
        kd, bd, max_err_d, mae_d, down_pred = fit_line(down)
        
        # 输出结果
        print(f"\n分析结果:")
        print(f"上升段斜率 = {ku:.2f}, 截距 = {bu:.2f}")
        print(f"上升段最大非线性 = {max_err_u:.2f}, MAE = {mae_u:.2f}")
        print(f"下降段斜率 = {kd:.2f}, 截距 = {bd:.2f}")
        print(f"下降段最大非线性 = {max_err_d:.2f}, MAE = {mae_d:.2f}")
        
        # 绘制结果
        plot_results(valid, up, down, up_pred, down_pred)
        print("\n分析图表已保存为 'slider_analysis.png'")
        
        # 保存处理后的数据
        valid.to_csv("slider_filtered_output.csv", index=False)
        print("处理后的数据已保存为 'slider_filtered_output.csv'")
        
    except Exception as e:
        print(f"错误: {str(e)}")

if __name__ == "__main__":
    main()
