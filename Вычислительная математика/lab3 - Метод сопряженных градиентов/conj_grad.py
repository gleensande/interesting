import numpy as np
import matplotlib.pyplot as plt

def conjugate_gradient(A, b, eps, C_inv=1):
    err = []

    # предобуславливание
    A_new = np.array(A.shape)
    if (type(C_inv) is int) and (C_inv == 1):
        A_new = A
    else:
        C_T = C_inv.transpose()
        A_new = C_inv.dot(A).dot(C_T)

    #начальные условия
    x_0 = [0] * A_new.shape[1]
    r_0 = b - np.dot(A_new, x_0)
    v_1 = r_0

    def conj_grad_recursive(x_prev, r_prev, v):
        err.append(norm(r_prev))
        if (norm(r_prev) < eps):
            return x_prev
        
        t = scal_prod(r_prev, r_prev) / scal_prod(v, A_new.dot(v))
        x = x_prev + (t * v)
        r = r_prev - (t * np.dot(A_new,v))
        s = scal_prod(r, r) / scal_prod(r_prev, r_prev)
        v_next = r + s * v

        return conj_grad_recursive(x, r, v_next)

    #вызов рекурсивного алгоритма
    return conj_grad_recursive(x_0, r_0, v_1), err

def norm(vec):
    norm = 0
    for v_i in vec:
        norm += v_i * v_i
    norm /= len(vec)
    norm = np.sqrt(norm)
    return norm

def scal_prod(vec1, vec2):
    if len(vec1) != len(vec2):
        print("lens don`t match")
    prod = 0
    for i in range(len(vec1)):
        prod += vec1[i] * vec2[i]
    return prod

def create_matrix_A(N):
    size = (N-1)
    A = np.zeros((size *size, size * size))

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

def pt_5():
    eps = 10 ** (-4)

    for N in [9, 17]:
        A = create_matrix_A(N)
        b = [1] * ((N-1)*(N-1))
        x, err = conjugate_gradient(A, b, eps)
        T = np.zeros((N+1,N+1))
        k = 0
        for i in range(1, N):
            for j in range(1, N):
                T[i][j] = x[k]
                k += 1
        level_lines_plot(T, N)

def level_lines_plot(z, N):
    x = []
    for i in range(z.shape[1]):
        x.append(i / N)
    y = x

    plt.figure(N)
    plt.grid()

    plt.xlabel('x')
    plt.ylabel('y')
    cs = plt.contourf(x, y, z)

    plt.clabel(cs, inline=1, fontsize=10)

    plt.savefig("plots/" + str(N) +".svg")
    plt.figure()

def pt_6():
    eps = 10 ** (-4)
    N = 17
    A = create_matrix_A(N)
    b = [1] * ((N-1)*(N-1))
    x, err = conjugate_gradient(A, b, eps)
    T = np.zeros((N+1,N+1))
    k = 0
    for i in range(1, N):
        for j in range(1, N):
            T[i][j] = x[k]
            k += 1
    err_plot(err, "Norm17_pt6")

def err_plot(err, name):
    fig, ax = plt.subplots()
    ax.grid()

    x_nodes_plot = range(len(err))
    y_nodes_plot = err
    line1, = ax.plot(x_nodes_plot, y_nodes_plot, label = 'Среднеквадратичная норма вектора невязки')

    dots1, = ax.plot(x_nodes_plot, y_nodes_plot, 'ro', markersize = 4, label = 'Значения на итерациях')

    plt.legend(handles = [line1, dots1])

    ax.set_xlabel('Номер итерации')
    ax.set_ylabel('Среднеквдратичная норма')

    plt.yscale('log')

    plt.savefig("plots/" + name + ".svg")
    plt.figure()

def pt_7():
    eps = 10 ** (-4)
    N = 17
    A = create_matrix_A(N)
    b = [1] * ((N-1)*(N-1))

    D = make_D_matrix(A)

    x, err = conjugate_gradient(A, b, eps, D)
    T = np.zeros((N+1,N+1))
    k = 0
    for i in range(1, N):
        for j in range(1, N):
            T[i][j] = x[k]
            k += 1
    err_plot(err, "Norm17_D")

def make_D_matrix(A):
    D = np.zeros(A.shape)
    for i in range(A.shape[0]):
        D[i][i] = - np.sqrt(A[i][i])
    return D

pt_7()
