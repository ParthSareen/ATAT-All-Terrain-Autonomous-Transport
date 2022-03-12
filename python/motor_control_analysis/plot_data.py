import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
import pandas as pd

csv_path = '/home/urbanpistek/Urban/at_at_mte380/ATAT-All-Terrain-Autonomous-Transport/python/motor_control_analysis/data/motor_speed_control_curve.csv'
df = pd.read_csv(csv_path)

# Rename columns
df.rename({'0': 'i', '157': 'rpm'}, axis=1, inplace=True)

print(df.head())
print(df.tail(20))

ypoints = df["rpm"]
print("Data Len: ", len(df["rpm"]))

plt.plot(ypoints)
plt.title("Motor Speed Control Curve")
plt.xlabel("Integer Input")
plt.ylabel("Reported RPM")
plt.show()

ypoints = df["rpm"][:238]
print("Filtered Len: ", len(df["rpm"][:238]))

plt.plot(ypoints)
plt.title("Motor Speed Control Curve Filtered")
plt.xlabel("Integer Input")
plt.ylabel("Reported RPM")
plt.show()

# Fit curve

plt.plot(ypoints)  # Plot base data


def func(x, a, b, c):
    """
    Function to fit to
    """
    return a * np.exp(-b * x) + c


xdata = np.linspace(0, 238, 238)
ydata = df["rpm"][:238]
popt, pcov = curve_fit(func, xdata, ydata)
plt.plot(xdata, func(xdata, *popt), 'r-',
         label='fit: a=%5.3f, b=%5.3f, c=%5.3f' % tuple(popt))

print("Fit: ", popt)

plt.title("Fitted Curve [a*exp(-b*x)+c] vs Motor Speed Control Curve")
plt.xlabel("Integer Input")
plt.ylabel("Reported RPM")
plt.legend()
plt.show()
