import pandas as pd
import matplotlib.pyplot as plt

df1 = pd.read_csv('fps_single.csv')
df2 = pd.read_csv('fps_multi.csv')

x1, y1 = df1.columns[0], df1.columns[1]
x2, y2 = df2.columns[0], df2.columns[1]

plt.figure(figsize=(8,5))

plt.plot(df1[x1], df1[y1], label='FPS Single')
plt.plot(df2[x2], df2[y2], label='FPS Multi')

plt.xlabel(df1.columns[0])
plt.ylabel(df1.columns[1])
plt.ylim([700, 900])
plt.title('FPS Single vs FPS Multi')
plt.legend()
plt.tight_layout()
plt.show()