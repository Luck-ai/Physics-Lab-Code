import matplotlib.pyplot as plt
import numpy as np
from scipy.ndimage import gaussian_filter1d

# Load CO2 data from file
file_path = 'pm1.txt'
with open(file_path, 'r') as file:
    co2_data = [int(line.strip()) for line in file]

# Apply offset adjustment: If value > 1100, mirror below 1100
adjusted_co2_data = [value if value <= 1100 else 1100 - (value - 1100) for value in co2_data]

# Compute average CO2 concentration per minute
num_minutes = len(adjusted_co2_data) // 60
average_co2 = [np.mean(adjusted_co2_data[i * 60:(i + 1) * 60]) for i in range(num_minutes)]
time_minutes = np.arange(num_minutes)

# Smooth the data using a Gaussian filter
#smoothed_co2 = gaussian_filter1d(average_co2, sigma=0.1)

# Define air purifier CO2 range
purifier_min = 14
purifier_max = 16.6

# Plot the data
plt.figure(figsize=(10, 5))
plt.plot(time_minutes, average_co2, label='Concentration', color='b', marker='o', markersize=4, linestyle='-', linewidth=2)
plt.axhline(y=purifier_min, color='r', linestyle='--', linewidth=1.5, label='Purifier Min (14)')
plt.axhline(y=purifier_max, color='g', linestyle='--', linewidth=1.5, label='Purifier Max (16.6)')

plt.xlabel('Time (minutes)')
plt.ylabel('PM2.5 Concentration')
plt.title('Average PM2.5 Concentration Over Time')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.6)
plt.show()