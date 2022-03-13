import os
import matplotlib.pyplot as plt
import numpy as np
from scipy.optimize import curve_fit
import pandas as pd
from pathlib import Path

CURR_DIR = Path(__file__).parent.absolute()

print(CURR_DIR)

csv_path = str(CURR_DIR)+'/data/motor_speed_control_values.csv'
df = pd.read_csv(csv_path)

# Print df
print(df.info())
print(df.head())
print(df.tail(20))

# Plot all raw data
x = df["i"]
mt_y = df["mt"]
m1_y = df["m1"]
m2_y = df["m2"]

plt.plot(x, mt_y, 'r')
plt.plot(x, m1_y, 'g')
plt.plot(x, m2_y, 'b')
plt.title("Motor Speed Control Curve - Unadjusted")
plt.xlabel("Integer Input")
plt.ylabel("Reported RPM")
plt.legend(['Test Motor', 'Motor 1', 'Motor 2'])
plt.show()
plt.savefig(str(CURR_DIR)+'/plots/unadjusted_all.png')

# Plot all adjusted data
x_adj = df["i"][:238]
mt_y_adj = df["mt"][:238]
m1_y_adj = df["m1"][:238]
m2_y_adj = df["m2"][:238]

plt.plot(x_adj, mt_y_adj, 'r')
plt.plot(x_adj, m1_y_adj, 'g')
plt.plot(x_adj, m2_y_adj, 'b')
plt.title("Motor Speed Control Curve - Adjusted")
plt.xlabel("Integer Input")
plt.ylabel("Reported RPM")
plt.legend(['Test Motor', 'Motor 1', 'Motor 2'])
plt.show()
plt.savefig(str(CURR_DIR)+'/plots/adjusted_all.png')

# Fit average data
def func(x, a, b, c):
    """
    Function to fit to
    """
    return a * np.exp(-b * x) + c

col = df.loc[: , "mt":"m2"]
df['m_mean'] = col.mean(axis=1)

xdata = np.linspace(0, 238, 238)
ydata = df['m_mean'][:238]
popt, pcov = curve_fit(func, xdata, ydata)
plt.plot(xdata, func(xdata, *popt), 'r-',
         label='fit: a=%5.3f, b=%5.3f, c=%5.3f' % tuple(popt))
plt.plot(x_adj, df['m_mean'][:238], 'b')

plt.title("Fitted Curve [a*exp(-b*x)+c] vs Mean Motor Speed Control Curve")
plt.xlabel("Integer Input")
plt.ylabel("Reported RPM")
plt.legend(['Fitted',' Mean'])
plt.show()
plt.savefig(str(CURR_DIR)+'/plots/fit_mean.png')
print("Fit: ", popt)

# Plot exponential fit
xdata = np.linspace(0, 252, 252)
plt.plot(xdata, func(xdata, *popt), 'r-',
         label='fit: a=%5.3f, b=%5.3f, c=%5.3f' % tuple(popt))

plt.title("Fitted Curve [a*exp(-b*x)+c] "+'fit: a=%5.3f, b=%5.3f, c=%5.3f' % tuple(popt))
plt.xlabel("Integer Input")
plt.ylabel("Calc RPM")
plt.legend('Fitted')
plt.show()
plt.savefig(str(CURR_DIR)+'/plots/fitted.png')