set terminal eps
set output "plotting/plot.eps"
set boxwidth 0.5 absolute

set style data boxplot
set title "Plotting with error bars"
set xlabel "Square matrix row size" font ",12"
set ylabel "Time in microseconds" font ",12"

plot "plotting/data1.txt" using 1:2:($2-$3):($2+$3) title "openblas" with errorbars , "plotting/data2.txt" using 1:2:($2-$3):($2+$3) title "mkl" with errorbars, "plotting/data3.txt" using 1:2:($2-$3):($2+$3) title "pthreads" with errorbars, "plotting/data.txt" using 1:2:($2-$3):($2+$3) title "normal" with errorbars
