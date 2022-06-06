target: yourcode.cpp
	g++ -I /usr/include/mkl yourcode.cpp -o yourcode.out -fopenmp -lmkl_intel_lp64 -lmkl_core -lmkl_gnu_thread -lpthread -lm -ldl

plots:
	./run.sh
