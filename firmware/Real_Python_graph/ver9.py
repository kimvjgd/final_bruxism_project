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

Debug_Mode = False

# Variables
ylim_EMG = 1023
ylim_Voltage = 8
ylim_Current = 30
ylim_Power = 100

ylim_EMG = ylim_EMG * 1.1
ylim_Voltage = ylim_Voltage * 1.1
ylim_Current = ylim_Current * 1.1
ylim_Power = ylim_Power * 1.1


class SerialPlotter:
    def __init__(self, master):
        self.master = master
        self.master.title("Serial Plotter")

        # Configure grid layout
        self.master.grid_rowconfigure(0, weight=0)
        self.master.grid_rowconfigure(1, weight=0)
        self.master.grid_rowconfigure(2, weight=0)
        self.master.grid_rowconfigure(3, weight=1)
        self.master.grid_columnconfigure(0, weight=5)
        self.master.grid_columnconfigure(1, weight=5)

        self.port_label = tk.Label(master, text="Select Port:")
        self.port_label.grid(row=0, column=0, sticky="ew")
        self.port_combobox = ttk.Combobox(master)
        self.port_combobox.grid(row=0, column=1, sticky="ew")

        self.baudrate_label = tk.Label(master, text="Select Baudrate:")
        self.baudrate_label.grid(row=1, column=0, sticky="ew")
        self.baudrate_combobox = ttk.Combobox(master, values=[9600, 115200, 230400])
        self.baudrate_combobox.grid(row=1, column=1, sticky="ew")
        self.baudrate_combobox.current(1)

        self.start_button = tk.Button(master, text="Start", command=self.start_plotting)
        self.start_button.grid(row=2, column=0, sticky="ew")

        self.stop_button = tk.Button(master, text="Stop", command=self.stop_plotting, state=tk.DISABLED)
        self.stop_button.grid(row=2, column=1, sticky="ew")

        self.plot_frame = tk.Frame(master)
        self.plot_frame.grid(row=3, column=0, columnspan=2, sticky="nsew")

        self.serial_thread = None
        self.running = False
        self.data_queue = queue.Queue()

        self.start_time = None
        self.plot_init()
        self.update_plot()

    def plot_init(self):
        self.fig, self.axs = plt.subplots(4, 1, figsize=(8, 12), sharex=True)  # 4개의 서브플롯 생성
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.plot_frame)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        self.xdata = deque(maxlen=100)  # x축 데이터를 최대 100개까지 유지
        self.ydata_emg = deque(maxlen=100)  # EMG 데이터를 최대 100개까지 유지
        self.ydata_voltage = deque(maxlen=100)  # Voltage 데이터를 최대 100개까지 유지
        self.ydata_current = deque(maxlen=100)  # Current 데이터를 최대 100개까지 유지
        self.ydata_power = deque(maxlen=100)  # Power 데이터를 최대 100개까지 유지

        self.lines = []
        labels = ['EMG', 'Voltage', 'Current', 'Power']
        colors = ['r', 'g', 'b', 'y']

        for ax, label, color in zip(self.axs, labels, colors):
            line, = ax.plot([], [], color + '-', label=label)
            ax.set_ylabel(label)
            self.lines.append(line)

        self.axs[0].set_ylim(0, ylim_EMG)  # EMG 값의 y축 범위 설정
        self.axs[1].set_ylim(0, ylim_Voltage)     # Voltage 값의 y축 범위 설정
        self.axs[2].set_ylim(0, ylim_Current)    # Current 값의 y축 범위 설정
        self.axs[3].set_ylim(0, ylim_Power)   # Power 값의 y축 범위 설정
        self.axs[-1].set_xlabel("Time (s)")

    def start_plotting(self):
        port = self.port_combobox.get()
        baudrate = int(self.baudrate_combobox.get())

        if not port or not baudrate:
            return

        # 데이터 초기화
        self.xdata.clear()
        self.ydata_emg.clear()
        self.ydata_voltage.clear()
        self.ydata_current.clear()
        self.ydata_power.clear()

        self.ser = serial.Serial(port, baudrate, timeout=0.1)
        self.running = True
        self.start_time = time.time()
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

        # 데이터 큐 초기화
        with self.data_queue.mutex:
            self.data_queue.queue.clear()

    def read_serial(self):
        data = []
        try:
            while self.running:
                sensor_value_str = self.ser.readline().decode('utf-8').strip()
                if sensor_value_str:
                    values = sensor_value_str.split(',')
                    if len(values) == 4:
                        try:
                            emg = float(values[0])
                            voltage = float(values[1])
                            current = float(values[2])
                            power = float(values[3])
                            elapsed_time = time.time() - self.start_time  # 시작 시간부터 경과 시간 계산
                            if Debug_Mode:
                                print(f"EMG: {emg}, Voltage: {voltage}, Current: {current}, Power: {power}")

                            # 큐에 데이터를 넣어 메인 스레드에서 처리하도록 함
                            self.data_queue.put((elapsed_time, emg, voltage, current, power))
                            current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')  # 현재 시간을 년도-월-일 시:분:초 형식으로 가져옴

                            # 데이터 저장
                            data.append((current_time, emg, voltage, current, power))

                        except ValueError:
                            print(f"잘못된 데이터 포맷입니다: {sensor_value_str}")
                    else:
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

        for line, ydata in zip(self.lines, [self.ydata_emg, self.ydata_voltage, self.ydata_current, self.ydata_power]):
            line.set_xdata(self.xdata)
            line.set_ydata(ydata)

        for ax in self.axs:
            ax.relim()  # 축 범위 업데이트
            ax.autoscale_view()  # 축 범위 자동으로 설정

        self.canvas.draw()
        self.canvas.flush_events()

        self.master.after(50, self.update_plot)

def find_serial_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

if __name__ == "__main__":
    root = tk.Tk()
    root.geometry("800x600")  # 윈도우 크기 설정
    root.grid_rowconfigure(0, weight=1)
    root.grid_columnconfigure(0, weight=1)
    plotter = SerialPlotter(root)
    plotter.port_combobox['values'] = find_serial_ports()
    root.mainloop()
