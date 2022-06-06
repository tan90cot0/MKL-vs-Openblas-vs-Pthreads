#!/bin/bash
rm "plotting/data1.txt"
rm "plotting/data2.txt"
rm "plotting/data3.txt"
rm "plotting/data.txt"
rm "plotting/datam1.txt"
rm "plotting/datam2.txt"
rm "plotting/datam3.txt"
rm "plotting/datam.txt"

for a in 40 80 120 160 200 240 280 320 360 400
do
	g++ -Werror matrix_gen.cpp
	if [[ $? == 0 ]]; then
	    ./a.out $a
	fi
	g++ -Werror -I /usr/include/mkl yourcode.cpp -o ./file.out -fopenmp -lmkl_intel_lp64 -lmkl_core -lmkl_gnu_thread -lpthread -lm -ldl
	if [[ $? == 0 ]]; then
	    ./file.out fullyconnected input.txt weights.txt biases.txt openblas outputmatrix_openblas.txt 
	    ./file.out fullyconnected input.txt weights.txt biases.txt mkl outputmatrix_mkl.txt
	    ./file.out fullyconnected input.txt weights.txt biases.txt pthreads outputmatrix_pthreads.txt
	    ./file.out fullyconnected input.txt weights.txt biases.txt normal outputmatrix_normal.txt
	fi
done
gnuplot plotting/plot.p
gnuplot plotting/plot_eps.p
gnuplot plotting/boxplot1.p
gnuplot plotting/boxplot1_eps.p
gnuplot plotting/boxplot2.p
gnuplot plotting/boxplot2_eps.p
gnuplot plotting/boxplot3.p
gnuplot plotting/boxplot3_eps.p
gnuplot plotting/boxplot4.p
gnuplot plotting/boxplot4_eps.p

rm "file.out"
rm "a.out"
