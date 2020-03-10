set logscale x
set logscale y
set grid
set xlabel "Количество узлов"
set ylabel "Относительная погрешность измерения"
set style line 1 lc rgb 'blue' lt 1 lw 2 pt 7 pi -1 ps 1.5
set style line 2 lc rgb 'red' lt 1 lw 2 pt 7 pi -1 ps 1.5
plot 'plot1.dat' using 1:2 with linespoints ls 1 title "|x|", \
	'plot2.dat' using 1:2 with linespoints ls 2 title "x*cos(x^2) + e^x*cos(e^x)"
