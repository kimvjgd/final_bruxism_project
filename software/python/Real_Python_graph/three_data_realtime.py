import tkinter as tk
from tkinter import ttk
from tkinter import messagebox
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
ylim_Sensors = [(1500, 2400 * 1.1), (0, 150 * 1.1), (1500, 2400 * 1.1)]  # 센서별 y축 범위 설정

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

        self.start_button = tk.Button(master, text="Start", command=self.start_plotting, bg="green", fg="white")
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

        # 값을 표시할 레이블 추가
        label_font = ("Arial", 16)
        self.sensor_labels = [
            tk.Label(master, text=f"Sensor {i+1} Value: N/A", font=label_font)
            for i in range(3)
        ]
        for i, label in enumerate(self.sensor_labels):
            label.grid(row=4 + i, column=0, columnspan=2, sticky="w")

        # 경고 메시지 박스 추가
        self.show_warning()

    def show_warning(self):
        messagebox.showwarning("Caution", "Push only Stop Button to record data")

    def plot_init(self):
        self.fig, self.axs = plt.subplots(3, 1, figsize=(8, 12), sharex=True)
        self.canvas = FigureCanvasTkAgg(self.fig, master=self.plot_frame)
        self.canvas.get_tk_widget().pack(fill=tk.BOTH, expand=True)

        self.xdata = deque(maxlen=100)
        self.ydata_sensors = [deque(maxlen=100) for _ in range(3)]

        self.lines = []
        for i, ax in enumerate(self.axs):
            line, = ax.plot([], [], label=f"Sensor {i+1} Value")
            ax.set_ylabel(f"Sensor {i+1} Value")
            ax.set_ylim(ylim_Sensors[i])
            ax.legend()
            self.lines.append(line)
        self.axs[-1].set_xlabel("Time (s)")

    def start_plotting(self):
        port = self.port_combobox.get()
        baudrate = int(self.baudrate_combobox.get())

        if not port or not baudrate:
            return

        self.xdata.clear()
        for ydata in self.ydata_sensors:
            ydata.clear()

        try:
            self.ser = serial.Serial(port, baudrate, timeout=0.1)
        except serial.SerialException as e:
            messagebox.showerror("Serial Error", f"Failed to open serial port: {e}")
            return

        self.running = True
        self.start_time = time.time()
        self.serial_thread = threading.Thread(target=self.read_serial)
        self.serial_thread.start()
        self.start_button.config(state=tk.DISABLED, bg=self.master.cget("bg"))
        self.stop_button.config(state=tk.NORMAL, bg="red", fg="white")

    def stop_plotting(self):
        self.running = False
        if self.serial_thread and self.serial_thread.is_alive():
            self.serial_thread.join()
        if hasattr(self, 'ser') and self.ser.is_open:
            self.ser.close()
        self.start_button.config(state=tk.NORMAL, bg="green", fg="white")
        self.stop_button.config(state=tk.DISABLED, bg=self.master.cget("bg"))

        with self.data_queue.mutex:
            self.data_queue.queue.clear()

    def read_serial(self):
        data = []
        try:
            while self.running:
                try:
                    line = self.ser.readline()
                    try:
                        decoded_line = line.decode('utf-8').strip()
                    except UnicodeDecodeError:
                        decoded_line = line.decode('latin-1').strip()
                    if decoded_line:
                        try:
                            values = [float(v) for v in decoded_line.split(',')]
                            if len(values) == 3:
                                elapsed_time = time.time() - self.start_time
                                if Debug_Mode:
                                    print(f"Sensor Values: {values}")

                                self.data_queue.put((elapsed_time, values))
                                current_time = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
                                data.append((current_time, *values))
                        except ValueError:
                            print(f"Invalid data format: {decoded_line}")
                except serial.SerialException as e:
                    print(f"Serial read error: {e}")
                    break
                time.sleep(0.01)
        except Exception as e:
            print(f"Unexpected error: {e}")
        finally:
            df = pd.DataFrame(data, columns=['Time', 'Sensor 1', 'Sensor 2', 'Sensor 3'])
            safe_time_str = datetime.now().strftime('%Y-%m-%d_%H-%M-%S')
            df.to_excel(f'{safe_time_str}.xlsx', index=False)
            print(f"Data saved to {safe_time_str}.xlsx")

    def update_plot(self):
        while not self.data_queue.empty():
            new_x, new_y_sensors = self.data_queue.get()
            self.xdata.append(new_x)
            for i, ydata in enumerate(self.ydata_sensors):
                ydata.append(new_y_sensors[i])

            for i, label in enumerate(self.sensor_labels):
                label.config(text=f"Sensor {i+1} Value: {new_y_sensors[i]} V")

        for i, (line, ax) in enumerate(zip(self.lines, self.axs)):
            line.set_xdata(self.xdata)
            line.set_ydata(self.ydata_sensors[i])
            ax.relim()
            ax.autoscale_view()

        self.canvas.draw()
        self.canvas.flush_events()

        self.master.after(50, self.update_plot)

def find_serial_ports():
    ports = serial.tools.list_ports.comports()
    return [port.device for port in ports]

if __name__ == "__main__":
    root = tk.Tk()
    root.geometry("800x600")
    root.grid_rowconfigure(0, weight=1)
    root.grid_columnconfigure(0, weight=1)
    plotter = SerialPlotter(root)
    plotter.port_combobox['values'] = find_serial_ports()
    root.mainloop()
