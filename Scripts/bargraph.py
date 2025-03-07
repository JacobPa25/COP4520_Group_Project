import pandas as pd
import matplotlib.pyplot as plt


df1 = pd.read_csv('fps_single.csv')
df2 = pd.read_csv('fps_multi.csv')

y1 = df1.columns[1]
y2 = df2.columns[1]

avg_single = df1[y1].mean()
avg_multi = df2[y2].mean()

labels = ['Single', 'Multi']
averages = [avg_single, avg_multi]

plt.figure(figsize=(8,5))
plt.bar(labels, averages, color=['blue', 'green'], width=.6)

plt.ylabel(df1.columns[1])
plt.title('Average FPS')

for i, v in enumerate(averages):
    plt.text(i, v + 1, f"{v:.2f}", ha='center')

plt.ylim([0, 1000])
plt.show()
