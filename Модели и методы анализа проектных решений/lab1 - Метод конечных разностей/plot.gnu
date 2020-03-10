set terminal png
set output "result.png"
set size ratio 0.5
set title "Температура пластины"

set xlabel "X"
set ylabel "Y"

set tic scale 0

set palette rgbformulae 10,13,22
set palette negative

set palette maxcolors 400
set cbrange [0:1100]
#unset cbtics

set xrange [0:10]
set yrange [0:10]

set view map

splot 'matrix.dat' matrix with image