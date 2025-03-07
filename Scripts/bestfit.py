import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

df1 = pd.read_csv('fps_single.csv')
df2 = pd.read_csv('fps_multi.csv')

x1, y1 = df1.columns[0], df1.columns[1]
x2, y2 = df2.columns[0], df2.columns[1]

df1_sorted = df1.sort_values(by=x1)
df2_sorted = df2.sort_values(by=x2)

plt.figure(figsize=(8,5))

plt.plot(df1[x1], df1[y1], label='FPS Single')
plt.plot(df2[x2], df2[y2], label='FPS Multi')


coeffs_single = np.polyfit(df1_sorted[x1], df1_sorted[y1], 1)
poly_single = np.poly1d(coeffs_single)
best_fit_single = poly_single(df1_sorted[x1])
plt.plot(df1_sorted[x1], best_fit_single, color='red', linestyle='--')


coeffs_multi = np.polyfit(df2_sorted[x2], df2_sorted[y2], 1)
poly_multi = np.poly1d(coeffs_multi)
best_fit_multi = poly_multi(df2_sorted[x2])
plt.plot(df2_sorted[x2], best_fit_multi, color='green', linestyle='--')


plt.xlabel(df1.columns[0])
plt.ylabel(df1.columns[1])
plt.ylim([700, 900])
plt.title('FPS Single vs FPS Multi')
plt.legend()
plt.tight_layout()
plt.show()
