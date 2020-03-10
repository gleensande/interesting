import splines as s
import data_parse as d
import numpy as np
import matplotlib.pyplot as plt

# получение опорных точек для интерполяции
time, temperature = d.get_data_from_csv()

# график по точкам замера температуры без интерполяции
fig, ax = plt.subplots()
ax.plot(time, temperature, 'bo', markersize=1)
ax.grid()
fig.autofmt_xdate()

ax.set_xlabel('Время измерения')
ax.set_ylabel('Температура (°C)')

plt.title("Точки интерполяции")

plt.savefig("plots/Точки.svg")
plt.figure()

# все точки
s.create_spline(time, temperature, 1)

# только в 3, 9, 15, 21 час
allowed = [3, 9, 15, 21]
s.create_spline(time, temperature, allowed)

# только в 3, 15 час
allowed = [3, 15]
s.create_spline(time, temperature, allowed)