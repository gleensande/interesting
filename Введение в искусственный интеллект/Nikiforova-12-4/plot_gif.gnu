reset
set term gif # animate delay 100
set output 'results.gif'
set size square

set border linewidth 1.5
set pointsize 1.5
set style line 1 lc rgb '#FD151B' pt 7   # circle

set border linewidth 1.5
set pointsize 1.5
set style line 2 lc rgb '#01295F' pt 7   # circle

unset key

set tics scale 0.1
set xtics 1
set ytics 1
set yrange[-3:4]
set xrange[-3:3]
set xlabel 'X_1' 
set ylabel 'X_2'

set macro

set style increment user

# N = system("wc -l weights.dat")

# do for [i=1:N] {
#     w_0 = system("head -" . i . " weights.dat | tail -1 | awk '{print $1}'")
#     w_1 = system("head -" . i . " weights.dat | tail -1 | awk '{print $2}'")
#     w_2 = system("head -" . i . " weights.dat | tail -1 | awk '{print $3}'")

#     LABEL = "i = " . i
#     set obj 10 rect at -2,0.5 size char strlen(LABEL), char 1 
#     set obj 10 fillstyle empty border -1 front
#     set label 10 at -2,0.5 LABEL front center

#     f(x) = -1/(w_2) * (w_1 * x + w_0)

    # plot f(x) w l ls 2, 'learn_data.dat' using 1:2:3 w p lc var, 'test_data.dat' using 1:2:3 w p lc var
# }

plot 'learn_data.dat' using 1:2 w p ls 1