import pandas as pd
import matplotlib.pyplot as plt

# Read the CSV file
data = pd.read_csv('kalman_output.csv')

# Calculate the errors
data['Angle Error'] = data['Correct Angle'] - data['Predicted Angle']
data['Angle Rate Error'] = data['Correct Angle Rate'] - data['Predicted Angle Rate']

# Plot Angles vs. Time
plt.figure(figsize=(12, 12))

plt.subplot(4, 1, 1)
plt.plot(data['Time'], data['Correct Angle'], label='Correct Angle')
plt.plot(data['Time'], data['Predicted Angle'], label='Predicted Angle')
plt.xlabel('Time (s)')
plt.ylabel('Angle (rad)')
plt.title('Angles vs. Time')
plt.legend()
plt.grid(True)

# Plot Angle Rates vs. Time
plt.subplot(4, 1, 2)
plt.plot(data['Time'], data['Correct Angle Rate'], label='Correct Angle Rate')
plt.plot(data['Time'], data['Predicted Angle Rate'], label='Predicted Angle Rate')
plt.xlabel('Time (s)')
plt.ylabel('Angle Rate (rad/s)')
plt.title('Angle Rates vs. Time')
plt.legend()
plt.grid(True)

# Plot Angle Errors vs. Time
plt.subplot(4, 1, 3)
plt.plot(data['Time'], data['Angle Error'], label='Angle Error')
plt.xlabel('Time (s)')
plt.ylabel('Angle Error (rad)')
plt.title('Angle Error vs. Time')
plt.legend()
plt.grid(True)

# Plot Angle Rate Errors vs. Time
plt.subplot(4, 1, 4)
plt.plot(data['Time'], data['Angle Rate Error'], label='Angle Rate Error')
plt.xlabel('Time (s)')
plt.ylabel('Angle Rate Error (rad/s)')
plt.title('Angle Rate Error vs. Time')
plt.legend()
plt.grid(True)

plt.tight_layout()
plt.show()
