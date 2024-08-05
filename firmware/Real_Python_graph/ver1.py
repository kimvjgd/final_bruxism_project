import serial
import time
import matplotlib.pyplot as plt
from collections import deque

# 그래프 초기화
plt.ion()  # 대화형 모드 설정: 실시간으로 그래프를 업데이트 할 수 있게 합니다.
fig, ax = plt.subplots()  # 그래프를 그리기 위한 figure와 axes를 생성합니다.
xdata = deque(maxlen=200)  # x축 데이터를 최대 100개까지 유지할 수 있는 deque를 생성합니다.
ydata = deque(maxlen=200)  # y축 데이터를 최대 100개까지 유지할 수 있는 deque를 생성합니다.
line, = ax.plot(xdata, ydata, 'r-')  # xdata와 ydata를 사용해 빨간색 선으로 빈 그래프를 생성합니다.
ax.set_ylim(0, 1100)  # y축의 범위를 0에서 1100까지로 설정합니다.

# 시리얼 포트 연결
ser = serial.Serial('COM8', 115200, timeout=0.1)  # COM 포트와 보레이트(통신 속도)를 설정해 시리얼 포트를 엽니다.

def update_plot(new_x, new_y):
    xdata.append(new_x)  # 새 x값을 xdata deque에 추가합니다.
    ydata.append(new_y)  # 새 y값을 ydata deque에 추가합니다.
    line.set_xdata(xdata)  # 갱신된 xdata를 그래프의 x축 데이터로 설정합니다.
    line.set_ydata(ydata)  # 갱신된 ydata를 그래프의 y축 데이터로 설정합니다.
    ax.relim()  # 축의 데이터 범위를 다시 설정합니다.
    ax.autoscale_view()  # 축의 범위를 자동으로 조정합니다.
    fig.canvas.draw()  # 그래프를 다시 그립니다.
    fig.canvas.flush_events()  # 그래프가 즉시 업데이트되도록 이벤트를 처리합니다.

def main():
    try:
        while True:
            # 센서값 읽기 (실제 센서 값 읽는 코드로 변경 필요)
            sensor_value_str = ser.readline().decode('utf-8').strip()  # 시리얼 포트에서 한 줄을 읽고, utf-8로 디코딩한 후 공백을 제거합니다.
            if sensor_value_str:  # 읽은 값이 비어있지 않으면
                try:
                    sensor_value = float(sensor_value_str)  # 문자열을 부동소수점 숫자로 변환합니다.
                    current_time = time.time()  # 현재 시간을 초 단위로 가져옵니다.
                    print(f"센서값: {sensor_value}")  # 센서값을 출력합니다.
                    
                    # 데이터 그래프 업데이트
                    update_plot(current_time, sensor_value)  # 그래프를 업데이트합니다.
                
                except ValueError:
                    print(f"잘못된 데이터 포맷입니다: {sensor_value_str}")  # 변환에 실패하면 오류 메시지를 출력합니다.
            
            # 10ms 대기
            time.sleep(0.01)  # 10밀리초 동안 대기합니다.
    
    except KeyboardInterrupt:
        print("프로그램을 종료합니다.")  # 키보드 인터럽트(Ctrl+C)를 감지하면 종료 메시지를 출력합니다.
        ser.close()  # 시리얼 포트를 닫습니다.

if __name__ == "__main__":
    main()  # main 함수를 실행합니다.
