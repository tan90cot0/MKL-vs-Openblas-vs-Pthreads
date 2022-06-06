							COP290 - Subtask 2
					   	  Arush Utkarsh and Aryan Dua
					   	            21/2/22
					   
					 	 Instructions to check the output:
1. Open the terminal inside this folder
2. Enter "make"
3. Type ./yourcode.out fullyconnected input.txt weights.txt biases.txt {openblas/mkl/pthreads/normal} outputmatrix.txt
4. The output matrix and dimensions will be stored in a file name outputmatrix.txt



						Instructions to plot runtime graphs:
1. Open the terminal inside this folder
2. Enter "make plots"
3. All the plots will be saved in a folder named plotting.


To access the plots, we need to navigate to a directory named "plotting"
There are 5 plots:(in .png and .eps formats)
1) plot.png - An errorbar plot that shows the runtimes of all 4 implementations.
2) boxplot_mkl.png - A boxplot of the runtimes of matrix multiplication implemented using mkl library
3) boxplot_openblas.png - A boxplot of the runtimes of matrix multiplication implemented using openblas library
4) boxplot_pthreads.png - A boxplot of the runtimes of matrix multiplication implemented using pthreads
5) boxplot_normal.png - A boxplot of the runtimes of matrix multiplication implemented using the normal O(n^3) implementation.



							Code Structure:-
								
1. The matrix multiplication using openblas is present in openblas.cpp, it is imported with the include statement.

2. The matrix multiplication using mkl is present in mkl.cpp, it is also imported with the include statement.

3. The normal matrix multiplication is present in yourcode.cpp.

4. We are writing the [size mean_runtime std_deviation_runtime] in files prefixed by "data". This file is used as input to the errorbar plot.

5. We are writing the [size min 25_percentile median 75_percentile max] in files prefixed by "datam". These are used for plotting the candlestick plots.

6. The makefile can take 2 inputs in the command line:
	1. make - it will compile the code, so that the executable file can be executed through the command line.
	2. make plots - it will remove unnecessary files, compile, execute the code, draw the plots and store them in a folder called plotting.
			It executes a bash script run.sh that performs all these tasks.
			
7. matrixgen.cpp is a cpp file that generates random matrices to test the rutimes of the implementations. It will take the size of the matrices as inputs and then generate random matrices corresponding to that size. It will write 3 files - input.txt, weights.txt and biases.txt.

8. 
*****Primary functions of pthreads and how to call them from command line:
	1. void matmul_pthreads(float out[], float inp[], float w[], float b[], int n1, int m3, int m1):
		This computes the matrix inp*w+b where 'inp', 'w' and 'b' are the input matrix, weight matrix and bias matrix respectively.
		Call this function by entering the following into the command line:	
			./yourcode.out fullyconnected inputmatrix.txt weightmatrix.txt biasmatrix.txt pthreads outputmatrix.txt

*****Secondary functions (utilized by the primary functions and cannot be called directly from command line):

	1. Functions in multpthread.cpp:
		
		a. void *threadMult( void *arg):
		
			This is the function that runs parallely in threads. It takes in an object of type struct matMulti as defined in multpthread.cpp which contains the number of rows and columns of the matrices, pointers to the matrices and a constructor function. It computes the value of one eighth of the elements of the product matrix.
			
		b. void matMultiply (int row1, int col1, vector<vector<float>> &input1, int col2, vector<vector<float>> &input2, vector<vector<float>> &output):
		
			This function takes in the matrices and their dimensions and creates 8 objects of type struct matMulti. It creates 8 threads and calls threadMult(). 
			
		c. matmul_pthreads(float out[], float inp[], float w[], float b[], int n1, int m3, int m1):
		
			This is the primary function which takes in the matrices in column major form and their dimensions and calls matMultiply() to obtain product matrix and finally performs addition of bias matrix. 
	
	
	
			
							Errors Handled:
								
1. If a wrong input command is entered, through the command line, it will gracefully catch the error and display how the input should be ideally entered.
2. If incompatible matrix dimensions are given.
3. File not found error.
4. If the number of command line arguments are not ideal.


							Inference from plots:
1. The libraries mkl and openblas are faster than our own pthreads implementation. For the sizes that we tested, the pthreads implementation took more time in general cases, just in a rare incident, the normal implementation runtime exceeds that of pthreads. Therefore, mkl~=openblas>>>normal>pthreads in terms of efficiency. (This was our observation from the plots).