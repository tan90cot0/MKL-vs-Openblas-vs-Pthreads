//#include "mkl_cblas.h"		
#include<iostream> 
#include <cmath> 
#include <cstring>
#include <fstream>  
#include <stdlib.h>
#include "mkl.cpp"
#include "openblas.cpp"
#include "multpthread.cpp"
#include <chrono>
#include <algorithm>
using namespace std;
using namespace std::chrono;

void sizeInp(int *shape, string filename)
{
    string srg;  
    ifstream filestream(filename);  
    if (filestream.is_open())  
    {  
        if(getline (filestream,srg))
            shape[0]= stoi(srg);
        if(getline (filestream,srg))
            shape[1]= stoi(srg); 
    }
    else    
    {
        cout << "File not found."<<endl;  
        exit(0);
    }
     
}

void fileInp(float *vector, string filename, int n)
{
    string srg;  
    ifstream filestream(filename);  
    if (filestream.is_open())  
    { 
        getline (filestream,srg);
        if(n==2)
            getline (filestream,srg);
        for(int i = 0;getline (filestream,srg);i++)  
            vector[i] = stod(srg);
    filestream.close();  
    }  
   else
   {
       cout << "File not found."<<endl; 
       exit(0);
   }    
}

void fileOut(int *shape, string filename, float *out, int n)
{
    ofstream myfile;
    myfile.open(filename);
    myfile<<shape[0]<<"\n";
    if(n==2)
        myfile<<shape[1]<<"\n";
    else
        shape[1] = 1;
    for(int i = 0;i<shape[0]*shape[1];i++)
        myfile<<out[i]<<"\n";
    myfile.close();    
}

void matmul(float *out, float *inp, float *w,float *b, int m, int l, int n)
{
    float inp2d[m][n];
    float w2d[n][l];
    float b2d[m][l];
    for(int i = 0;i<m;i++)
        for(int j = 0;j<n;j++)
            inp2d[j][i] = inp[m*j+i];
    
    for(int i = 0;i<n;i++)
        for(int j = 0;j<l;j++)
            w2d[i][j] = w[n*j+i];
    
    for(int i = 0;i<m;i++)
        for(int j = 0;j<l;j++)
            b2d[i][j] = b[m*j+i];

    for(int i = 0;i<m;i++)
        for(int j = 0;j<l;j++)
        {
            float sum = 0;
            for(int k = 0;k<n;k++)
                    sum+=inp2d[k][i]*w2d[k][j];
            out[m*j+i] = sum + b2d[i][j];
        }

}

int main(int argc, char *argv[])
{
	int num_iters = 100;
	float time[num_iters];
	long long int size;
	for(int p = 0;p<num_iters;p++)
	{
	auto start = high_resolution_clock::now();
    if(argc==1) cout<<"Please enter the files through the command line arguments.\n";
    else if(!strcmp(argv[1],"fullyconnected") && argc==7)
    {
        int shape[2];
        sizeInp(shape, argv[2]);
        int m1 = shape[0];
        int n1 = shape[1];
        sizeInp(shape, argv[3]);
        int m2 = shape[0];
        int n2 = shape[1];
        sizeInp(shape, argv[4]);
        int m3 = shape[0];
        int n3 = shape[1];
        
        size = m1;
        if(n1>0 && m1>0  && n2>0 && n2==m1 && m3==m2 && n1==n3)
        {
            float inp[n1*m1], w[n2*m2], b[n3*m3], out[n1*m2];
            fileInp(inp, argv[2], 2);
            fileInp(w, argv[3], 2);
            fileInp(b, argv[4], 2);
            if(!strcmp(argv[5],"openblas"))
            	matmul_openblas(out, inp,w, b,n1, m3, m1);
            else if(!strcmp(argv[5],"mkl"))
            	matmul_mkl(out, inp,w, b,n1, m3, m1);				//change this to matmul_openblas
            else if(!strcmp(argv[5],"pthreads"))
            	matmul_pthreads(out, inp,w, b,n1, m3, m1);
            else
            	matmul(out, inp,w, b,n1, m3, m1);
            //out array is in col major form  
            shape[1] = n1;
            shape[0] = m2;
            fileOut(shape, argv[6], out, 2);
        }
        else
            cout<<"Please enter valid dimensions:- If input is (m,n) and w is (n,o) then b is (m,o) and output is (m,o).\n";

    }
    else
        cout<<"Please enter fullyconnected after ./yourcode.out. Then the respective matrices(in text files), then the matrix multiplication implementation and finally the output text file.\n";
            auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start); 
    time[p] = duration.count();
    }
    sort(time, time+num_iters);
    double mean;
    double variance = 0;
    double stdev;
    double sum = 0;
    double median = time[49];
    double quart1 = time[24];
    double quart3 = time[74];
    double min = time[0];
    double max = time[99];
    
    for(int i = 0;i<num_iters;i++)
    	sum += time[i];
    mean = sum/num_iters;
    
    for(int i = 0;i<num_iters;i++)
	variance += pow(time[i] - mean, 2);
    variance=variance/num_iters;
    stdev = sqrt(variance);

    std::ofstream outfile;
    string name;
    if(!strcmp(argv[5],"openblas"))
    	name = "plotting/data1.txt";
    else if (!strcmp(argv[5],"mkl"))
    	name = "plotting/data2.txt";
    else if(!strcmp(argv[5],"pthreads"))
    	name = "plotting/data3.txt";
    else
    	name = "plotting/data.txt";
    outfile.open(name, std::ios_base::app); // append instead of overwrite
    outfile << size << " "<< mean<<" "<<stdev<<endl;
    
    string name2;
    if(!strcmp(argv[5],"openblas"))
    	name2 = "plotting/datam1.txt";
    else if (!strcmp(argv[5],"mkl"))
    	name2 = "plotting/datam2.txt";
    else if (!strcmp(argv[5],"pthreads"))
    	name2 = "plotting/datam3.txt";
    else
    	name2 = "plotting/datam.txt";
    
    
    std::ofstream outfile2;
    outfile2.open(name2, std::ios_base::app); // append instead of overwrite
    outfile2 << size << " "<< min<<" "<<quart1 <<" "<<median <<" "<<quart3 <<" "<<max <<endl;
}
