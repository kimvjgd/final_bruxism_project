import serial
import time
from datetime import datetime
import matplotlib.pyplot as plt
from collections import deque
import pandas as pd

Debug_Mode = True

# 그래프 초기화
plt.ion()  # 대화형 모드 설정
fig, ax = plt.subplots()
xdata = deque(maxlen=100)  # x축 데이터를 최대 100개까지 유지
ydata_emg = deque(maxlen=100)  # EMG 데이터를 최대 100개까지 유지
ydata_voltage = deque(maxlen=100)  # Voltage 데이터를 최대 100개까지 유지
ydata_current = deque(maxlen=100)  # Current 데이터를 최대 100개까지 유지
ydata_power = deque(maxlen=100)  # Power 데이터를 최대 100개까지 유지
line_emg, = ax.plot(xdata, ydata_emg, 'r-', label='EMG')  # 빈 그래프 생성
line_voltage, = ax.plot(xdata, ydata_voltage, 'g-', label='Voltage')
line_current, = ax.plot(xdata, ydata_current, 'b-', label='Current')
line_power, = ax.plot(xdata, ydata_power, 'y-', label='Power')
ax.set_ylim(0, 1100)
ax.legend()

# 시리얼 포트 연결
ser = serial.Serial('COM8', 115200, timeout=0.1)  # COM 포트와 보레이트 설정

# 데이터를 저장할 리스트
data = []

def update_plot(new_x, new_y_emg, new_y_voltage, new_y_current, new_y_power):
    xdata.append(new_x)
    ydata_emg.append(new_y_emg)
    ydata_voltage.append(new_y_voltage)
    ydata_current.append(new_y_current)
    ydata_power.append(new_y_power)
    line_emg.set_xdata(xdata)
    line_emg.set_ydata(ydata_emg)
    line_voltage.set_xdata(xdata)
    line_voltage.set_ydata(ydata_voltage)
    line_current.set_xdata(xdata)
    line_current.set_ydata(ydata_current)
    line_power.set_xdata(xdata)
    line_power.set_ydata(ydata_power)
    ax.relim()  # 축 범위 업데이트
    ax.autoscale_view()  # 축 범위 자동으로 설정
    fig.canvas.draw()
    fig.canvas.flush_events()

def main():
    try:
        while True:
            # 센서값 읽기
            if Debug_Mode:
                print(ser.readline())
            sensor_value_str = ser.readline().decode('utf-8').strip()
            if sensor_value_str:
                try:
                    emg, voltage, current, power = map(float, sensor_value_str.split(','))
                    current_time_sec = time.time()  # 현재 시간을 초 단위로 가져옴
                    if not Debug_Mode:
                        print(f"EMG: {emg}, Voltage: {voltage}, Current: {current}, Power: {power}")
                    
                    # 데이터 그래프 업데이트
                    update_plot(current_time_sec, emg, voltage, current, power)
                    current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # 현재 시간을 년도-월-일 시:분:초 형식으로 가져옴
                    
                    # 데이터 저장
                    data.append((current_time, emg, voltage, current, power))
                
                except ValueError:
                    print(f"잘못된 데이터 포맷입니다: {sensor_value_str}")
            
            # 10ms 대기
            time.sleep(0.01)
    
    except KeyboardInterrupt:
        print("프로그램을 종료합니다.")
        ser.close()
        # 데이터를 pandas DataFrame으로 변환
        df = pd.DataFrame(data, columns=['Time', 'EMG', 'Voltage', 'Current', 'Power'])
        # 엑셀 파일로 저장
        safe_time_str = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        df.to_excel(f'{safe_time_str}.xlsx', index=False)
        print(f"데이터를 {safe_time_str}.xlsx 파일로 저장했습니다.")

if __name__ == "__main__":
    main()
