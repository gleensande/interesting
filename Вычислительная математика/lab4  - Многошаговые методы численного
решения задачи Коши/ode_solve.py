import numpy as np
import matplotlib.pyplot as plt

def create_matrix_A(N):
    size = (N-1)
    A = np.zeros((size * size, size * size))

    E = -N * N * np.eye(size)

    D = np.zeros((size, size))
    diag_el = 4 * N * N
    near_el = -N * N
    D[0][0] = diag_el
    D[0][1] = near_el
    for i in range(1, size-1):
        D[i][i] = diag_el
        D[i][i+1] = near_el
        D[i][i-1] = near_el
    D[size-1][size-1] = diag_el
    D[size-1][size-2] = near_el

    m = 0
    for k in range(N-1):
        put_in_matrix(A, D, m, m)
        m += N-1

    m = 0
    for k in range(N-2):
        put_in_matrix(A, E, m, m + N-1)
        put_in_matrix(A, E, m + N-1, m)
        m += N-1

    return A

def put_in_matrix(A, M, i, j):
    for k in range(M.shape[0]):
        for m in range(M.shape[1]):
            A[i+k][j+m] = M[k][m]

def ab4(T_prev, delta_t, A, f, i):
    a = np.array([55/24, -59/24, 37/24, -9/24])
    # правильный вид T_prev = [Tn, Tn-1, Tn-2, (Tn-3)]

    sum = 0
    for k in range(4):
        sum += a[k] * (np.dot(-A, T_prev[i-k-1]) + f)

    return (T_prev[i-1] + delta_t * sum)

def rk4(T_prev, delta_t, A, f):
    k1 = (np.dot(-A, T_prev) + f)
    k2 = (np.dot(-A, T_prev + (delta_t / 2) * k1) + f)
    k3 = (np.dot(-A, T_prev + (delta_t / 2) * k2) + f)
    k4 = (np.dot(-A, T_prev + delta_t * k3) + f)
    
    return (T_prev + (delta_t/6) * (k1 + 2 * k2 + 2 * k3 + k4))

def ode_solve(f, t_final, delta_t, N):
    m = int(t_final / delta_t)
    T = np.zeros((m + 1, len(f)))
    T_mean = []
    t_mean = []

    A = create_matrix_A(N)

    t = 0
    for i in range (1, 4):
        T[i] = rk4(T[i-1], delta_t, A, f)
        t += delta_t
        if i == 2:
            level_lines_plot(T[i], N, delta_t, t)
        t_mean.append(t)
        T_mean.append(np.mean(T[i]))

    for i in range (4, m + 1):
        T[i] = ab4(T, delta_t, A, f, i)
        t += delta_t
        if i == 5 or i == 10 or i == 100:
            level_lines_plot(T[i], N, delta_t, t)
        t_mean.append(t)
        T_mean.append(np.mean(T[i]))

    mean_plot(T_mean, t_mean, "Средняя температура OK")

    return T[m]

def pt_5():
    N = 17
    f = [1] * ((N-1)*(N-1))
    t_final = 0.3

    delta_t_max = 0.001

    x = ode_solve(f, t_final, delta_t_max, N)
    T = np.zeros((N+1,N+1))
    k = 0
    for i in range(1, N):
        for j in range(1, N):
            T[i][j] = x[k]
            k += 1
    level_lines_plot(T, N, delta_t_max)

def level_lines_plot(z, N, d, time):
    T = np.zeros((N+1,N+1))
    k = 0
    for i in range(1, N):
        for j in range(1, N):
            T[i][j] = z[k]
            k += 1

    x = []
    for i in range(T.shape[1]):
        x.append(i / N)
    y = x

    plt.figure(N)
    plt.grid()

    plt.xlabel('x')
    plt.ylabel('y')
    cs = plt.contour(x, y, T)

    plt.clabel(cs, inline=1, fontsize=10)

    plt.savefig("plots/" + str(d) + " " + str(time) +".svg")
    plt.figure()

def mean_plot(T, t, name):
    fig, ax = plt.subplots()
    ax.grid()

    x_nodes_plot = t
    y_nodes_plot = T
    line1, = ax.plot(x_nodes_plot, y_nodes_plot, label = 'Средняя температура')

    #dots1, = ax.plot(x_nodes_plot, y_nodes_plot, 'ro', markersize = 4, label = 'Результаты итераций')

    plt.legend(handles = [line1])

    ax.set_xlabel('Время')
    ax.set_ylabel('Средняя температура')

    #plt.yscale('log')

    plt.savefig("plots/" + name + ".svg")
    plt.figure()

def pt_6():
    N = 17
    f = [1] * ((N-1)*(N-1))
    t_final = 0.3

    delta_t_max = 0.0001

    x = ode_solve(f, t_final, delta_t_max, N)


pt_6()