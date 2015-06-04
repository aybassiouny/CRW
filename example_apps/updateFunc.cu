#include "updateFunc.h"

static void HandleError( cudaError_t err,
                         const char *file,
                         int line ) {
    if (err != cudaSuccess) {
        printf( "%s in %s at line %d\n", cudaGetErrorString( err ),
                file, line );
        exit( EXIT_FAILURE );
    }
}
#define HANDLE_ERROR( err ) (HandleError( err, __FILE__, __LINE__ ))

__global__ void setup_kernel(curandState *state){

  unsigned int seed = (unsigned int) clock64();
  int idx = threadIdx.x+blockDim.x*blockIdx.x;
  curand_init(seed, idx, 0, &state[idx]);
}

__global__ void update2(int *inc, int *outc, int *rpt, int *res, int *mxrptV , int *numgoodV, curandState *my_curandstate ) 
{
	int curVer = blockIdx.x * blockDim.x + threadIdx.x;
	//for (int curVer = 0; curVer<*numgoodV; curVer++){
		for (int i = 0; i<rpt[curVer]; i++){
			//int *edge_rand = new int;
			//*edge_rand = rand() % outc[curVer];
            float myrandf = curand_uniform(my_curandstate+curVer);
            myrandf *= (outc[curVer] - 0+0.999999);
            myrandf += 0;
            int edge_rand = (int)truncf(myrandf);
			res[curVer*(*mxrptV) + i] = edge_rand;
			//atomicAdd(rpt+(edge_rand), 1);
		}
	//}
}


void callUpdate2(int *inc, int *outc, int *minisch, int *res, int mxrptV , int numgoodV )
{
    curandState *d_state;
    cudaMalloc(&d_state, sizeof(curandState));
    setup_kernel<<<1,1>>>(d_state);
    
	int *inc_d, *outc_d, *minisch_d, *res_d, *mxrptV_d, *numgoodV_d;
                
    HANDLE_ERROR( cudaMalloc( (void**)&inc_d, numgoodV * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&outc_d, numgoodV * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&minisch_d, numgoodV * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&res_d, numgoodV*mxrptV * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&mxrptV_d, 1 * sizeof(int) ) );
    HANDLE_ERROR( cudaMalloc( (void**)&numgoodV_d, 1 * sizeof(int) ) );
    
    HANDLE_ERROR( cudaMemcpy( inc_d, inc, numgoodV * sizeof(int), cudaMemcpyHostToDevice ) );
    HANDLE_ERROR( cudaMemcpy( outc_d, outc, numgoodV * sizeof(int), cudaMemcpyHostToDevice ) );
    HANDLE_ERROR( cudaMemcpy( minisch_d, minisch, numgoodV * sizeof(int), cudaMemcpyHostToDevice ) );
    HANDLE_ERROR( cudaMemcpy( res_d, res,  numgoodV*mxrptV * sizeof(int), cudaMemcpyHostToDevice ) );
    HANDLE_ERROR( cudaMemcpy( mxrptV_d, &mxrptV, 1 * sizeof(int), cudaMemcpyHostToDevice ) );
    HANDLE_ERROR( cudaMemcpy( numgoodV_d, &numgoodV, 1 * sizeof(int), cudaMemcpyHostToDevice ) );
    
    update2 <<< 500,500 >>> (inc_d,  outc_d,  minisch_d,  res_d,  mxrptV_d, numgoodV_d, d_state);
    
    HANDLE_ERROR( cudaMemcpy( inc, inc_d, numgoodV * sizeof(int), cudaMemcpyDeviceToHost ) );
    HANDLE_ERROR( cudaMemcpy( outc, outc_d, numgoodV * sizeof(int), cudaMemcpyDeviceToHost ) );
    HANDLE_ERROR( cudaMemcpy( minisch, minisch_d, numgoodV * sizeof(int), cudaMemcpyDeviceToHost ) );
    HANDLE_ERROR( cudaMemcpy( res, res_d,  numgoodV*mxrptV * sizeof(int), cudaMemcpyDeviceToHost ) );
    HANDLE_ERROR( cudaMemcpy( &mxrptV, mxrptV_d, 1 * sizeof(int), cudaMemcpyDeviceToHost ) );
    HANDLE_ERROR( cudaMemcpy( &numgoodV, numgoodV_d, 1 * sizeof(int), cudaMemcpyDeviceToHost ) );
    
	cudaFree( inc_d );
	cudaFree( outc_d );
	cudaFree( minisch_d );
	cudaFree( res_d );
	cudaFree( mxrptV_d );
	cudaFree( numgoodV_d );
}

