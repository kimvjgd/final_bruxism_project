import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
import serial
import time
from datetime import datetime
import matplotlib.pyplot as plt
from collections import deque
import pandas as pd
import threading
import serial.tools.list_ports
import queue

Debug_Mode = True

class SerialPlotter:
    def __init__(self, master):
        self.master = master
        self.master.title("Serial Plotter")

        self.port_label = tk.Label(master, text="Select Port:")
        self.port_label.grid(row=0, column=0)
        self.port_combobox = ttk.Combobox(master)
        self.port_combobox.grid(row=0, column=1)

        self.baudrate_label = tk.Label(master, text="Select Baudrate:")
        self.baudrate_label.grid(row=1, column=0)
        self.baudrate_combobox = ttk.Combobox(master, values=[9600, 115200, 230400])
        self.baudrate_combobox.grid(row=1, column=1)
        self.baudrate_combobox.current(1)

        self.start_button = tk.Button(master, text="Start", command=self.start_plotting)
        self.start_button.grid(row=2, column=0, columnspan=2)

        self.stop_button = tk.Button(master, text="Stop", command=self.stop_plotting, state=tk.DISABLED)
        self.stop_button.grid(row=3, column=0, columnspan=2)

        self.plot_frame = tk.Frame(master)
        self.plot_frame.grid(row=4, column=0, columnspan=2)

        self.serial_thread = None
        self.running = False
        self.data_queue = queue.Queue()

        self.plot_init()
        self.update_plot()

    def plot_init(self):
        self.fig, self.ax = plt.subplots()
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.plot_frame)
        self.canvas.get_tk_widget().pack()
        self.xdata = deque(maxlen=100)  # x축 데이터를 최대 100개까지 유지
        self.ydata_emg = deque(maxlen=100)  # EMG 데이터를 최대 100개까지 유지
        self.ydata_voltage = deque(maxlen=100)  # Voltage 데이터를 최대 100개까지 유지
        self.ydata_current = deque(maxlen=100)  # Current 데이터를 최대 100개까지 유지
        self.ydata_power = deque(maxlen=100)  # Power 데이터를 최대 100개까지 유지
        self.line_emg, = self.ax.plot(self.xdata, self.ydata_emg, 'r-', label='EMG')  # 빈 그래프 생성
        self.line_voltage, = self.ax.plot(self.xdata, self.ydata_voltage, 'g-', label='Voltage')
        self.line_current, = self.ax.plot(self.xdata, self.ydata_current, 'b-', label='Current')
        self.line_power, = self.ax.plot(self.xdata, self.ydata_power, 'y-', label='Power')
        self.ax.set_ylim(0, 1100)
        self.ax.legend()

    def start_plotting(self):
        port = self.port_combobox.get()
        baudrate = int(self.baudrate_combobox.get())

        if not port or not baudrate:
            return

        self.ser = serial.Serial(port, baudrate, timeout=0.1)
        self.running = True
        self.serial_thread = threading.Thread(target=self.read_serial)
        self.serial_thread.start()
        self.start_button.config(state=tk.DISABLED)
        self.stop_button.config(state=tk.NORMAL)

    def stop_plotting(self):
        self.running = False
        if self.serial_thread.is_alive():
            self.serial_thread.join()
        self.ser.close()
        self.start_button.config(state=tk.NORMAL)
        self.stop_button.config(state=tk.DISABLED)

    def read_serial(self):
        data = []
        try:
            while self.running:
                sensor_value_str = self.ser.readline().decode('utf-8').strip()
                if sensor_value_str:
                    try:
                        emg, voltage, current, power = map(float, sensor_value_str.split(','))
                        current_time_sec = time.time()  # 현재 시간을 초 단위로 가져옴
                        if not Debug_Mode:
                            print(f"EMG: {emg}, Voltage: {voltage}, Current: {current}, Power: {power}")

                        # 큐에 데이터를 넣어 메인 스레드에서 처리하도록 함
                        self.data_queue.put((current_time_sec, emg, voltage, current, power))
                        current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # 현재 시간을 년도-월-일 시:분:초 형식으로 가져옴

                        # 데이터 저장
                        data.append((current_time, emg, voltage, current, power))

                    except ValueError:
                        print(f"잘못된 데이터 포맷입니다: {sensor_value_str}")

                # 10ms 대기
                time.sleep(0.01)
        except serial.SerialException:
            print("Serial connection error.")
        finally:
            # 데이터를 pandas DataFrame으로 변환
            df = pd.DataFrame(data, columns=['Time', 'EMG', 'Voltage', 'Current', 'Power'])
            # 엑셀 파일로 저장
            safe_time_str = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
            df.to_excel(f'{safe_time_str}.xlsx', index=False)
            print(f"데이터를 {safe_time_str}.xlsx 파일로 저장했습니다.")

    def update_plot(self):
        while not self.data_queue.empty():
            new_x, new_y_emg, new_y_voltage, new_y_current, new_y_power = self.data_queue.get()
            self.xdata.append(new_x)
            self.ydata_emg.append(new_y_emg)
            self.ydata_voltage.append(new_y_voltage)
            self.ydata_current.append(new_y_current)
            self.ydata_power.append(new_y_power)
            self.line_emg.set_xdata(self.xdata)
            self.line_emg.set_ydata(self.ydata_emg)
            self.line_voltage.set_xdata(self.xdata)
            self.line_voltage.set_ydata(self.ydata_voltage)
            self.line_current.set_xdata(self.xdata)
            self.line_current.set_ydata(self.ydata_current)
            self.line_power.set_xdata(self.xdata)
            self.line_power.set_ydata(self.ydata_power)
            self.ax.relim()  # 축 범위 업데이트
            self.ax.autoscale_view()  # 축 범위 자동으로 설정
            self.canvas.draw()
            self.canvas.flush_events()

        self.master.after(100, self.update_plot)


def find_serial_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]


if __name__ == "__main__":
    root = tk.Tk()
    plotter = SerialPlotter(root)
    plotter.port_combobox['values'] = find_serial_ports()
    root.mainloop()
