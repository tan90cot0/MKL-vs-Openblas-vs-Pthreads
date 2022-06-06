#include <iostream>
#include <fstream>
#include <vector>
#include <stdlib.h>
#include <pthread.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

struct matMulti {
	int thread_id;
	int row1, col1, col2;
	
	vector<vector<float>> *A;
	vector<vector<float>> *B;
	vector<vector<float>> *C;
	matMulti(int r1, int c1, vector<vector<float>> &i1, int c2, vector<vector<float>> &i2, vector<vector<float>> &o){
		thread_id = 0;
		row1 = r1;
		col1 = c1;
		col2 = c2;
		A = &i1;
		B = &i2;
		C = &o;
	}
}; 

void *threadMult( void *arg)
{
     struct matMulti *temp = (struct matMulti *) arg;
     int count = temp->thread_id;
     
     while (count<temp->row1*temp->col2){
     	int div = count/temp->col2;
     	int mod = count%temp->col2;
     	float sum = 0;
     	for (int i=0; i<temp->col1;i++){
     		sum += (*(temp->A))[div][i]*((*(temp->B))[i][mod]);
     	}
     	(*(temp->C))[div][mod] = sum;
     	count+=8;
     }
     pthread_exit(NULL);
}
void matMultiply (int row1, int col1, vector<vector<float>> &input1, int col2, vector<vector<float>> &input2, vector<vector<float>> &output){
	struct matMulti passThread[8] = { matMulti(row1, col1, input1, col2, input2, output), matMulti(row1, col1, input1, col2, input2, output), matMulti(row1, col1, input1, col2, input2, output), matMulti(row1, col1, input1, col2, input2, output), matMulti(row1, col1, input1, col2, input2, output), matMulti(row1, col1, input1, col2, input2, output), matMulti(row1, col1, input1, col2, input2, output), matMulti(row1, col1, input1, col2, input2, output)};
	pthread_t threads[8];
	for (int i=0; i<8; i++)
		passThread[i].thread_id = i;
	for (int j=0; j<8; j++){
		struct matMulti *p = &passThread[j];
		pthread_create(&threads[j], NULL, threadMult, (void*)(p));
	}
	for (int k=0; k<8; k++)
		pthread_join(threads[k], NULL);
}	

void matmul_pthreads(float out[], float inp[], float w[], float b[], int n1, int m3, int m1){
	int x,y;
	vector<vector<float>> input(n1, vector<float>(m1));
	vector<vector<float>> weight(m1, vector<float>(m3));
	vector<vector<float>> bias(n1, vector<float>(m3));
	vector<vector<float>> output(n1, vector<float>(m3));
	for (x=0;x<m1;x++){
		for (y=0;y<n1;y++){
			input[y][x] = inp[x*n1 + y];
		}
	}
	for (x=0;x<m3;x++){
		for (y=0;y<m1;y++){
			weight[y][x] = w[x*m1 + y];
		}
	}
	for (x=0;x<m3;x++){
		for (y=0;y<n1;y++){
			bias[y][x] = b[x*n1 + y];
		}
	}
	matMultiply(n1,m1,input,m3,weight,output);
	for (x=0;x<m3;x++){
		for (y=0;y<n1;y++){
			output[y][x] = output[y][x] + bias[y][x];
			out[x*n1 + y] = output[y][x];
		}
	}
}
	
	

/*int main(){
	int row1 = 2000;
	int col1 = 2000;
	int col2 = 2000;
	/*vector<vector<float>> A(row1, vector<float>(col1));
	vector<vector<float>> B(col1, vector<float>(col2));
	float arr1[4] = {1,3,2,4};
	float arr2[4] = {4,2,3,1};
	float arr3[4] = {10,15,20,25};
	float arr4[4];
	matmul_pthreads(arr4, arr1, arr2, arr3, 2, 2, 2);
	for (int u=0;u<4;u++)
		cout<<arr4[u]<<" ";
	cout<<endl;
	/*for (int a=0;a<row1;a++){
		for (int b=0;b<col1;b++){
			A[a][b] = rand()%1000000000;
			B[a][b] = rand()%1000000000;
		}
	}
	vector<vector<float>> C(row1, vector<float>(col2));
	auto start = high_resolution_clock::now();
	matMultiply(row1, col1, A, col2, B, C);
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<nanoseconds>(stop - start);
	cout << duration.count() << endl;
	/*for (int i=0; i<row1; i++) {
		for (int j=0; j<col2; j++)
			cout<<C[i][j]<< " ";
		cout<<endl;
	}
	return 0;
}*/

