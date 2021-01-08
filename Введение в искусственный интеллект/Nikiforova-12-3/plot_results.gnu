set terminal pngcairo size 1418,1017 enhanced font 'Verdana,10'
set output 'results.png'

# Styling
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
set yrange[0:5]
set xrange[-3:6]
set xlabel 'X_1' 
set ylabel 'X_2'


set grid

# эти данные заменяются вручную из результатов программы
w_00 = 4.77
w_01 = 6.83
w_02 = -4.94

w_10 = 5.83
w_11 = 4.69
w_12 = -5.14

w_20 = 2.06
w_21 = 8.44
w_22 = -4.6

f0(x) = -1/(w_02) * (w_01 * x + w_00)
f1(x) = -1/(w_12) * (w_11 * x + w_10)
f2(x) = -1/(w_22) * (w_21 * x + w_20)


set style increment user
plot f0(x) w l, f1(x) w l, f2(x) w l, 'learn_data.dat' using 1:2:3 w p lc var, 'test_data.dat' using 1:2:3 w p lc var