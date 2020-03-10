import numpy as np
import random as rand
import matplotlib.pyplot as plt

def poly_regression(x_nodes, y_nodes, degree):
    n = degree
    m = len(x_nodes)

    X = np.ones((m, n+1))  
    for i in range(0, m):
        for j in range(1, n+1):
            X[i][j] = x_nodes[i] ** j

    Xt = np.transpose(X)
 
    a = np.matmul(np.matmul(np.linalg.inv(np.matmul(Xt,X)),Xt), y_nodes)
    return a


def f(x, X=0, sigma=0):
    return (-10*(x**2) + 1.5*x + 1 + sigma*X)


def get_plot_nodes(start, stop, num_of_points, func):
    num_of_plot_points = 2000
    x_nodes_plot = np.linspace(start, stop, num_of_plot_points)
    
    y_nodes_plot = []
    for i in range(num_of_plot_points):
        y_nodes_plot.append(func(x_nodes_plot[i]))


    return x_nodes_plot, y_nodes_plot


def analysis():
    xregr = [0] * 7
    Xregr = [0] * 7
    xtest = [0] * 7
    Xtest = [0] * 7
    Dregr = [0] * 7
    Dtest = [0] * 7
    errregr = [0] * 7
    errtest = [0] * 7

    for i in range(3, 10):
        N = 2 ** i
        for s in range(-2, 3):
            sigma = 10 ** s
            errregr[i - 3] = [0] * 5
            errtest[i - 3] = [0] * 5
            for p in range (1, 6):
                # задание исходных значений
                Xregr[i - 3] = [0] * N
                Xtest[i - 3] = [0] * N
                xregr[i - 3] = [0] * N
                xtest[i - 3] = [0] * N
                Dregr[i - 3] = [0] * N
                Dtest[i - 3] = [0] * N
                errregr[i - 3][s + 2] = [0] * 5
                errtest[i - 3][s + 2] = [0] * 5


                for j in range(N):
                    Xregr[i - 3][j] = np.random.randn() * 2 + -1
                    Xtest[i - 3][j] = np.random.randn() * 2 + -1

                    xregr[i - 3][j] = np.random.rand() * 2 + -1
                    xtest[i - 3][j] = np.random.rand() * 2 + -1
                for j in range(N):
                    Dregr[i - 3][j] = f(xregr[i - 3][j], Xregr[i - 3][j], sigma)
                    Dtest[i - 3][j] = f(xtest[i - 3][j], Xtest[i - 3][j], sigma)

                # вывод графиков для сравнения
                polyregr = poly_regression(xregr[i-3], Dregr[i-3], p)
                def polynom(x):
                    result = 0
                    for i in range(len(polyregr)):
                        result += x**i * polyregr[i]
                    return result

                '''fig, ax = plt.subplots()
                ax.grid()

                x_nodes_plot, y_nodes_plot = get_plot_nodes(-1, 1, 100, polynom)
                line1, = ax.plot(x_nodes_plot, y_nodes_plot, label = 'Полином')


                x_nodes_plot, y_nodes_plot = get_plot_nodes(-1, 1, 100, f)
                line2, = ax.plot(x_nodes_plot, y_nodes_plot, label = 'Исходная функция')

                dots1, = ax.plot(xregr[i-3], Dregr[i-3] , 'ro', markersize = 4, label = 'Начальные точки')
                dots2, = ax.plot(xtest[i-3], Dtest[i-3] , 'co', markersize = 4, label = 'Проверочные точки')
            
                plt.legend(handles = [line1, line2, dots1, dots2])

                ax.set_xlabel('x')
                ax.set_ylabel('f(x)')
            
                plt.savefig("plots/N" + str(N) + "_s" + str(sigma) + "_p" + str(p) + ".svg")
                plt.figure()'''

                # подсчет погрешностей
                errregr[i - 3][s + 2][p - 1] = 0
                for k in range(len(Dregr[i-3])):
                    errregr[i - 3][s + 2][p - 1] += (Dregr[i-3][k] - polynom(xregr[i-3][k])) ** 2

                errregr[i - 3][s + 2][p - 1] = np.sqrt(errregr[i - 3][s + 2][p - 1]) / len(Dregr[i-3])
                #print("N=", N, " sigma=", sigma, " p=", p, " errReg=", errregr[i - 3][s + 2][p - 1])

                errtest[i - 3][s + 2][p - 1] = 0
                for k in range(len(Dtest[i-3])):
                    errtest[i - 3][s + 2][p - 1] += (Dtest[i-3][k] - polynom(xtest[i-3][k])) ** 2
                
                errtest[i - 3][s + 2][p - 1] = np.sqrt(errtest[i - 3][s + 2][p - 1]) / len(Dtest[i-3])
                #print("N=", N, " sigma=", sigma, " p=", p, " errTest=", errtest[i - 3][s + 2][p - 1])

    print(errregr)

    # графики погрешностей в зависимости от N
    '''x_nodes = [2**3, 2**4, 2**5, 2**6, 2**7, 2**8, 2**9]
    y_nodesregr = []
    y_nodestest = []
    for s in range(-2, 3):
        for p in range (1, 6):
            y_nodesregr = [0] * 7
            y_nodestest = [0] * 7
            for i in range (3, 10): 
                y_nodesregr[i - 3] = errregr[i - 3][s + 2][p - 1]
            
            fig, ax = plt.subplots()
            ax.grid()
            
            line, = ax.plot(x_nodes, y_nodesregr, label = 'Среднеквадратичная погрешность')
            
            plt.legend(handles = [line])

            ax.set_xlabel('N')
            ax.set_ylabel('Погрешность')

            plt.savefig("plots/var_N, s=" + str(2**s) + "p=" + str(p) + ".svg")
            plt.figure()'''



analysis()

