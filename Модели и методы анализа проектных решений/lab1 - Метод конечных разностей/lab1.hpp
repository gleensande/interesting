#ifndef LAB1_HPP
#define LAB1_HPP

// 10x10
// максимальные номера узлов сетки, минимум - 0 узел
#define I_MAX 10
#define J_MAX 10
#define K_MAX 10

// шаги сетки в cм || с
#define D_X 0.5
#define D_Y 0.5
#define D_T 1

// начальные условия
#define T_0 20

// граничные условия
#define T_LEFT(neighbour) (neighbour)
#define T_BOTTOM() (20.0)
#define T_RIGHT() (800.0)
#define T_UP() (800.0)

#endif  // LAB1_HPP