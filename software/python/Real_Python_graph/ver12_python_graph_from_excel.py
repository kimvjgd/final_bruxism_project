import pandas as pd
import matplotlib.pyplot as plt

# Load the new Excel file
file_path = 'sample.xlsx'
data = pd.read_excel(file_path)

# Generate a synthetic 'Time' column with 19 data points per second
num_points = len(data)
time_intervals = pd.date_range(start='2024-01-01', periods=num_points, freq='52.63ms')

# Assign this time_intervals to the 'Time' column in the data
data['Time'] = time_intervals

# Function to calculate y-axis limits with padding
def get_limits(values, padding_ratio=0.1):
    min_val = values.min()
    max_val = values.max()
    padding = (max_val - min_val) * padding_ratio
    return min_val - padding, max_val + padding

# Plot each component over time in a 2x2 grid
components = ['EMG', 'Voltage', 'Current', 'Power']
fig, axes = plt.subplots(2, 2, figsize=(15, 10))

for i, component in enumerate(components):
    ax = axes[i // 2, i % 2]
    if component == 'EMG':
        ax.plot(data['Time'], data[component], label=component)
    else:
        ax.scatter(data['Time'], data[component], label=component, s=10)  # s is the marker size
    ax.set_xlabel('Time')
    ax.set_ylabel(component)
    ax.set_title(f'Time vs {component}')
    ax.legend()
    ax.grid(True)
    y_min, y_max = get_limits(data[component])
    ax.set_ylim(y_min, y_max)

plt.tight_layout()
plt.show()
