set terminal png
set output 'res/result.png'

# Set linestyle 1 to blue (#0060ad)
set style line 1 \
    linecolor rgb '#0060ad' \
    linetype 1 linewidth 2 \
    pointtype 7 pointsize 0.1

plot 'res/result.dat' with linespoints linestyle 1