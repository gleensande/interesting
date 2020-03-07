#include <stdio.h>

#ifndef LIFT_TERMINAL_H
#define LIFT_TERMINAL_H

#define ESC "\033"
#define abs(N) (((N) < 0) ? (-(N)) : ((N)))
#define clrscr() printf(ESC "[2J") //lear the screen, move to (1,1)
#define gotoxy(x, y) printf(ESC "[%d;%dH", y, x);
#define set_display_atrib(color) printf(ESC "[%dm", color)
#define resetcolor() printf(ESC "[0m")

#define B_BLACK 40
#define B_RED 41

#endif  // LIFT_TERMINAL_H