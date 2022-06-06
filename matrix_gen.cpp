#include<iostream> 
#include<fstream> 
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<string.h>
using namespace std;

int randn(int n)
{
    return rand() % n + 1;
}

int main(int argc, char *argv[])
{
    srand(time(NULL));
    int m = stoi(argv[1]);
    //int m = randn(50);
    int n = m;
    int k = m;

    std::ofstream outfile1;
    outfile1.open("input.txt");
    outfile1 <<k<<endl;
    outfile1 <<m<<endl;
    for(int i = 0; i<k*m; i++)
        outfile1 << randn(100)<<endl; 

    std::ofstream outfile2;
    outfile2.open("weights.txt");
    outfile2 <<n<<endl;
    outfile2 <<k<<endl;
    for(int i = 0; i<k*n; i++)
        outfile2 << randn(100)<<endl; 

    std::ofstream outfile3;
    outfile3.open("biases.txt");
    outfile3 <<n<<endl;
    outfile3 <<m<<endl;
    for(int i = 0; i<n*m; i++)
        outfile3 << randn(100)<<endl; 
    return 0;
}

