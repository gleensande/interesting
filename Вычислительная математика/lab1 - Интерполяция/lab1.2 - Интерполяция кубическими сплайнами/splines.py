import numpy as np
import matplotlib.pyplot as plt
from datetime import datetime, timedelta
from itertools import groupby

# находит коэффициенты кубического сплайна по точкам
def cubic_spline_coeff(x_nodes, y_nodes):
    if len(x_nodes) != len(y_nodes):
        print("lens of x_nodes and y_nodes dont match")
        # return

    n = len(y_nodes)

    # a coefficients, size = n
    a = y_nodes

    # h coefficients, size = n - 1
    h = np.empty(n - 1)
    for i in range(n - 1):
        h[i] = x_nodes[i + 1].timestamp() - x_nodes[i].timestamp()

    # left diag of matrix A
    left_diag = np.zeros(n - 1)
    for i in range(n - 2):
        left_diag[i] = h[i]

    # center diag of matrix A
    center_diag = np.ones(n)
    for i in range(1, n - 1):
        center_diag[i] = 2 * (h[i - 1] + h[i])

    # right diag of matrix A
    right_diag = np.zeros(n - 1)
    for i in range(1, n - 1):
        right_diag[i] = h[i]

    # matrix A
    A = np.diag(left_diag, -1) +  np.diag(center_diag, 0) + np.diag(right_diag, 1)

    # matrix B
    B = np.zeros((n, 1))
    for i in range (1, n - 1):
        B[i][0] = (3 / h[i]) * (a[i + 1] - a[i]) - (3 / h[i - 1]) * (a[i] - a[i - 1])

    # matrix of coeffs c computation
    c = np.linalg.solve(A, B)

    # matrix b
    b = np.zeros(n - 1)
    for i in range(n - 1):
        b[i] = (a[i + 1] - a[i]) / h[i] - (h[i] / 3) * (c[i + 1] + 2 * c[i])

    # matrix d
    d = np.zeros(n - 1)
    for i in range(n - 1):
        d[i] = (c[i + 1] - c[i]) / (3 * h[i])

    # matrix of all coeffs
    qs_coeff = np.zeros((n - 1, 4))

    for i in range(n - 1):
        qs_coeff[i][0] = a[i]
        qs_coeff[i][1] = b[i]
        qs_coeff[i][2] = c[i]
        qs_coeff[i][3] = d[i]

    # returns matrix of (a, b, c, d) coeffs
    # TODO(gleensande) : is a needed?
    return qs_coeff

# создает функцию, вычисляющую значение сплайна в точке x
def qubic_spline(x_nodes, qs_coeff):
    a = qs_coeff[:, 0]
    b = qs_coeff[:, 1]
    c = qs_coeff[:, 2]
    d = qs_coeff[:, 3]

    n = len(x_nodes)

    # преобразуем полученные абсциссы к числовому виду
    x_nodes_t = []
    for i in range(len(x_nodes)):
        x_nodes_t.append(x_nodes[i].timestamp())

    def qubic_spline_by_x(x):
        # преобразуем дату к числовому виду
        x_t = x.timestamp()
        i = 0
        for j in range(n - 1):
            if (x_t >= x_nodes_t[j]) and (x_t <= x_nodes_t[j + 1]):
                i = j
                break

        diff = x_t - x_nodes_t[i]
        s = a[i] + b[i] * diff + c[i] * (diff ** 2) + d[i] * (diff ** 3)

        return s

    return qubic_spline_by_x

# создает функцию, вычисляющую значение производной сплайна в точке x
def d_qubic_spline(x_nodes, qs_coeff):
    b = qs_coeff[:, 1]
    c = qs_coeff[:, 2]
    d = qs_coeff[:, 3]

    n = len(x_nodes)

    # преобразуем полученные абсциссы к числовому виду
    x_nodes_t = []
    for i in range(len(x_nodes)):
        x_nodes_t.append(x_nodes[i].timestamp())

    def d_qubic_spline_by_x(x):
        # преобразуем дату к числовому виду
        x_t = x.timestamp()
        i = 0
        for j in range(n - 1):
            if (x_t >= x_nodes_t[j]) and (x_t <= x_nodes_t[j + 1]):
                i = j
                break

        diff = x_t - x_nodes_t[i]
        s = b[i] + 2 * c[i] * diff + 3 * d[i] * (diff ** 2)

        return s

    return d_qubic_spline_by_x

# получить только разрешенные точки
def get_allowed_points(time, temperature, allowed):
    allowed_time = []
    allowed_temperature = []
    for i in range(len(time)):
        if time[i].hour in allowed:
            allowed_time.append(time[i])
            allowed_temperature.append(temperature[i])

    return allowed_time, allowed_temperature

# создаёт функцию сплайна по значениям x и y, используя ранее определенные функции
def get_qubic_spline_func(x_nodes, y_nodes):
    qs_coeff = cubic_spline_coeff(x_nodes, y_nodes)
    qubic_spline_func = qubic_spline(x_nodes, qs_coeff)

    return qubic_spline_func

# получение точек для построения графика интерполяции сплайнами
def get_plot_nodes(start, stop, num_of_points, qubic_spline_func):
    num_of_plot_points = 2000
    x_nodes_plot_t = np.linspace(start.timestamp(), stop.timestamp(), num_of_plot_points)
    
    # возврат обратно к виду даты
    x_nodes_plot = []
    for i in range(len(x_nodes_plot_t)):
        x_nodes_plot.append(datetime.fromtimestamp(x_nodes_plot_t[i]))
    
    y_nodes_plot= []
    for i in range(num_of_plot_points):
        y_nodes_plot.append(qubic_spline_func(x_nodes_plot[i]))


    return x_nodes_plot, y_nodes_plot

# создает график точек интерполяции и переданных значений в отдельном файле
def create_spline(time_all, temperature_all, allowed = 1):
    # получение только нужных точек для интерполяции
    if allowed != 1:
        time, temperature = get_allowed_points(time_all, temperature_all, allowed)
    else :
        time = time_all
        temperature = temperature_all

    # определение количества узлов интерполяции
    num_of_points = len(time)

    # получение функции для температуры
    temp_func = get_qubic_spline_func(time, temperature)
    x_nodes_plot, y_nodes_plot = get_plot_nodes(time[0], time[-1], num_of_points, temp_func)

    # построение графика интерполяции по всем переданным точкам
    fig, ax = plt.subplots()
    fig.autofmt_xdate()
    plt.title("Температура в Нью-Йорке (интерполяция)")
    ax.grid()

    spline, = ax.plot(x_nodes_plot, y_nodes_plot, label = 'Интерполяция')
    dots, = ax.plot(time, temperature , 'ro', markersize = 1, label = 'Точки интерполяции')
    plt.legend(handles = [spline, dots])

    ax.set_xlabel('Время измерения')
    ax.set_ylabel('Температура (°C)')
   
    plt.savefig("plots/Интерполяция по " + str(num_of_points) + " точкам.svg")
    plt.figure()

    if allowed == 1:
        return

    # вычисление нормы по всем точкам
    norm = 0
    i_n = 0
    for i in range(len(time_all) - 1):
        # в сокращенной выборке нет предсказания на не входящие туда внешние точки
        if time_all[i].timestamp() > time[-1].timestamp():
            break

        diff = abs(temperature_all[i] - temp_func(time_all[i]))
        if diff > norm:
            norm = diff
            i_n = i

    print("\nДля интерполяции по " + str(num_of_points) + " точкам")
    print("Норма = ", norm)
    print("Соответствует времени измерения ", time_all[i_n])
    print("Значение интерполирующей функции в этой точке: ", temp_func(time_all[i_n]))
    print("Реальное значение: ", temperature_all[i_n])

    # вычисление нормы по среднесуточной температуре

    # все дни, которые есть в списке
    days_all_not_distinct = []
    for i in range(len(time_all)):
        days_all_not_distinct.append(time_all[i].day)

    # выделяем уникальные дни
    days_all_distinct = [el for el, _ in groupby(days_all_not_distinct)]

    # подсчет исходной среднесуточной температуры
    temps_all_distinct = [0] * (len(days_all_distinct))
    n_day = 0
    n_of_mesures_per_day = 0
    for i in range(len(time_all)):
        if time_all[i].day == days_all_distinct[n_day]:
            temps_all_distinct[n_day] += temperature_all[i]
            n_of_mesures_per_day += 1
        else:
            temps_all_distinct[n_day] = temps_all_distinct[n_day] / n_of_mesures_per_day
            n_day += 1
            n_of_mesures_per_day = 0
    temps_all_distinct[n_day] = temps_all_distinct[n_day] / n_of_mesures_per_day

    print("\nУникальные дни измерений:\n", days_all_distinct)
    print("\nСредняя температура в каждый из дней по измерениям:\n", temps_all_distinct)

    # подсчет интерполированной среднесуточной температуры
    temps_distinct = [0] * (len(days_all_distinct))
    for i in range(len(days_all_distinct)):
        # исключение внешних непредсказанных точек по дням
        if days_all_distinct[i] > time[-1].day:
            break

        #генерация списка точек измерения в текущий день и подсчет функции в каждой из них
        some_times_in_day = []
        some_times_in_day.append(datetime(2018, 5, days_all_distinct[i], 0))
        real_num_of_points = 0
        for j in range(10):
            some_times_in_day.append(some_times_in_day[j] + timedelta(hours=2))
            # исключение внешних точек по времени
            if some_times_in_day[j].timestamp() > time[-1].timestamp():
                break
            temps_distinct[i] += temp_func(some_times_in_day[j])
            real_num_of_points += 1
        if real_num_of_points == 10:
            temps_distinct[i] += temp_func(some_times_in_day[10])

        temps_distinct[i] /= real_num_of_points

    print("\nСредняя температура в каждый из дней по интерполяции:\n", temps_distinct)

     # вычисление нормы по всем точкам
    norm = 0
    i_n = 0
    for i in range(len(temps_distinct)):
        diff = abs(temps_distinct[i] - temps_all_distinct[i])
        if diff > norm:
            norm = diff
            i_n = i

    print("Норма по средней температуре = ", norm)
    print("Значение интерполирующей функции в этой точке: ", temps_distinct[i_n])
    print("Реальное значение: ", temps_all_distinct[i_n])
