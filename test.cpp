

#include "mex.h"

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
	mexPrintf("Hello World\n");

	double *A,*B;
	int M,N;
	
	A = mxGetPr(prhs[0]);
	
	M = mxGetM(prhs[0]);
	N = mxGetN(prhs[0]);
	plhs[0] = mxCreateDoubleMatrix(M, N, mxREAL);
	
	B = mxGetPr(plhs[0]);
	
	for(int n =0; n < N; n++)
	{
		for(int m = 0; m < M; m++)
		{
			mexPrintf("Print Value: %d \n" , A[m + M*n]);
			B[m + M*n] = A[m + M*n];
		}
	}
	
	
	return;
}