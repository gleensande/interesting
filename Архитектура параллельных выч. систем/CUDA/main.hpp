#ifndef MAIN_HPP
#define MAIN_HPP

// размер пластины в см и время в с
#define N 5.0
#define M 5.0
#define TIME_MAX 10.0

// максимальные номера узлов сетки, минимум - 0 узел
#define I_MAX 10
#define J_MAX 10
#define K_MAX 10

// шаги сетки в cм || с
#define D_X (N / I_MAX)
#define D_Y (M / J_MAX)
#define D_T (TIME_MAX / K_MAX)


// начальные условия
#define T_0 20

// граничные условия
#define T_LEFT(neighbour) (neighbour)
#define T_BOTTOM() (800.0)
#define T_RIGHT() (800.0)
#define T_UP() (800.0)

#endif // MAIN_HPP