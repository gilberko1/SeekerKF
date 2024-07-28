import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
data = pd.read_csv('kalman_output.csv')

# Plot Angles vs. Time
plt.figure(figsize=(12, 6))

plt.subplot(2, 1, 1)
plt.plot(data['Time'], data['Correct Angle'], label='Correct Angle')
plt.plot(data['Time'], data['Predicted Angle'], label='Predicted Angle')
plt.xlabel('Time (s)')
plt.ylabel('Angle (degrees)')
plt.title('Angles vs. Time')
plt.legend()
plt.grid(True)

# Plot Angle Rates vs. Time
plt.subplot(2, 1, 2)
plt.plot(data['Time'], data['Correct Angle Rate'], label='Correct Angle Rate')
plt.plot(data['Time'], data['Predicted Angle Rate'], label='Predicted Angle Rate')
plt.xlabel('Time (s)')
plt.ylabel('Angle Rate (degrees/s)')
plt.title('Angle Rates vs. Time')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
