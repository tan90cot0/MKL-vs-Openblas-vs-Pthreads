#include "mkl_cblas.h"
void matmul_mkl(float * c,  float * a,  float * b, float *d, int m, int n, int k)
{
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m, n, k, 1.0, a, m, b,k, 0.0, c, m);
  for(int i = 0;i<m*n;i++)
  	c[i] = c[i] + d[i];
}
