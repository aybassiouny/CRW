#ifndef UPDATE_FUNC_HEADER_
#define UPDATE_FUNC_HEADER_

#include <string>
#include <utility>
#include <algorithm> 
#include <cuda_runtime.h>
#include <curand.h>
#include <curand_kernel.h>
#include <math.h>
#include <stdio.h>

__global__ void update2(int *inc, int *outc, int *rpt, int *res, int *mxrptV , int *numgoodV );
void callUpdate2(int *inc, int *outc, int *rpt, int *res, int mxrptV , int numgoodV );

#endif