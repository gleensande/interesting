set terminal pngcairo size 1418,1017 enhanced font 'Verdana,10'
set output 'learn_data.png'

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

w_0 = 4.77
w_1 = 6.83
w_2 = -4.94

f(x) = -1/(w_2) * (w_1 * x + w_0)

# Plot some points 

set style increment user
plot f(x) w l ls 2, 'learn_data.dat' using 1:2:3 w p lc var