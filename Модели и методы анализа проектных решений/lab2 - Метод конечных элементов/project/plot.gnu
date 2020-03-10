set terminal png
set output 'res/result.png'

l_1=(5+sqrt(17))/4
l_2=(5-sqrt(17))/4
c_1=-13.9
c_2=117
f(x)=c_1*exp(l_1*x)+c_2*exp(l_2*x)
y(x)=0.256687*(45.6155*2.71828**(0.219224*x+1.03078)+12*2.71828**(0.219224*x+3.31155)-4.38447*2.71828**(2.28078*x+1.03078)-12*2.71828**(2.28078*x+1.25))

set style line 1 \
    linecolor rgb '#1f77b4' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 0.1

set style line 2 \
    linecolor rgb '#ff7f0e' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 0.1

set style line 3 \
    linecolor rgb '#2ca02c' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 0.1

set style line 4 \
    linecolor rgb '#110000' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 0.1

set xlabel 'x'
set ylabel 'u(x)'
set grid
set key left bottom

plot 'res/result_N1.dat' title 'N = 20' with linespoints linestyle 1, \
     'res/result_N2.dat' title 'N = 40' with linespoints linestyle 2, \
     f(x) title 'true' with linespoints linestyle 3#, \
     #'res/additional_N2.dat' with linespoints linestyle 4
