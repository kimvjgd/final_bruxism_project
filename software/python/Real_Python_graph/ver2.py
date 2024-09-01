import serial
import time
from datetime import datetime
import matplotlib.pyplot as plt
from collections import deque
import pandas as pd

# 그래프 초기화
plt.ion()  # 대화형 모드 설정
fig, ax = plt.subplots()
xdata = deque(maxlen=100)  # x축 데이터를 최대 100개까지 유지
ydata = deque(maxlen=100)  # y축 데이터를 최대 100개까지 유지
line, = ax.plot(xdata, ydata, 'r-')  # 빈 그래프 생성
ax.set_ylim(0, 1100)

# 시리얼 포트 연결
ser = serial.Serial('COM8', 115200, timeout=0.1)  # COM 포트와 보레이트 설정

# 데이터를 저장할 리스트
data = []

def update_plot(new_x, new_y):
    xdata.append(new_x)
    ydata.append(new_y)
    line.set_xdata(xdata)
    line.set_ydata(ydata)
    ax.relim()  # 축 범위 업데이트
    ax.autoscale_view()  # 축 범위 자동으로 설정
    fig.canvas.draw()
    fig.canvas.flush_events()

def main():
    try:
        while True:
            # 센서값 읽기 (실제 센서 값 읽는 코드로 변경 필요)
            sensor_value_str = ser.readline().decode('utf-8').strip()
            if sensor_value_str:
                try:
                    sensor_value = float(sensor_value_str)
                    current_time_sec = time.time()  # 현재 시간을 초 단위로 가져옴

                    print(f"센서값: {sensor_value}")
                    
                    # 데이터 그래프 업데이트
                    update_plot(current_time_sec, sensor_value)
                    current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # 현재 시간을 년도-월-일 시:분:초 형식으로 가져옴
                    
                    
                    # 데이터 저장
                    data.append((current_time, sensor_value))
                
                except ValueError:
                    print(f"잘못된 데이터 포맷입니다: {sensor_value_str}")
            
            # 10ms 대기
            time.sleep(0.01)
    
    except KeyboardInterrupt:
        print("프로그램을 종료합니다.")
        ser.close()
        # 데이터를 pandas DataFrame으로 변환
        df = pd.DataFrame(data, columns=['Time', 'SensorValue'])
        # 엑셀 파일로 저장
        safe_time_str = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
        df.to_excel(f'{safe_time_str}.xlsx', index=False)
        print("데이터를 sensor_data.xlsx 파일로 저장했습니다.")

if __name__ == "__main__":
    main()
