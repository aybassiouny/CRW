#include "updateFunc.h"

const int CUDA_MAX_BLOCKS =65535;
const int CUDA_MAX_THREADS =1024;
const int CUDA_MAX_V = 100000;

//const int CUDA_MAX_THREADS_PER_BLOCK =65535;

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

__global__ void setup_kernel(curandState *state, int *numgoodV){
  int idx = blockIdx.x * blockDim.x + threadIdx.x;
  if(idx<*numgoodV){
      unsigned int seed = (unsigned int) clock64()+idx;
      curand_init(seed, idx, 0, &state[idx]);
  }
}

__global__ void update2(int *inc, int *outc, int *rpt, int *res, int *resloc, int *numgoodV, int *offset, curandState *my_curandstate) 
{
    int curVer = blockIdx.x * blockDim.x + threadIdx.x;
    //printf("Currently doing thread %d %d %d\n", blockIdx.x, blockDim.x, threadIdx.x);
    if(curVer<*numgoodV)
    {
        for (int i = 0; i<rpt[curVer]; i++){
            
            float myrandf = curand_uniform(my_curandstate+curVer);
            myrandf *= (outc[curVer]-1 - 0+0.999999);
            myrandf += 0;
            res[resloc[curVer]-*offset+i] = (int)truncf(myrandf);
        }
    }
}


void callUpdate2(int *inc, int *outc, int *minisch, int *res, int* resloc , int totressz, int totgoodV )
{
    int remgoodV = totgoodV;
    int offset=0;
    int ressz =0;
    //for(int curiter = 0; curiter<=niter; curiter++)
    while(remgoodV>0)
    {
        assert(offset<totgoodV);
        int numgoodV = min(remgoodV, CUDA_MAX_V);
        int resloc_offset = resloc[offset];
        ressz = resloc[numgoodV+offset-1]+minisch[numgoodV+offset-1] - 
            (resloc[offset]);
        std::cout<<numgoodV<<" "<<ressz<<" "<<offset<<" "<<std::endl;
        curandState *d_state;
        cudaMalloc(&d_state, numgoodV*sizeof(curandState));
        int numThreads  = 256;
        int numBlocks = (numgoodV+numThreads-1)/numThreads;  
        
        assert(numThreads<=CUDA_MAX_THREADS);
        assert(numBlocks<=CUDA_MAX_BLOCKS);

        

        int *inc_d, *outc_d, *minisch_d, *res_d, *resloc_d, *numgoodV_d, *offset_d;
                    
        HANDLE_ERROR( cudaMalloc( (void**)&inc_d, numgoodV * sizeof(int) ) );
        HANDLE_ERROR( cudaMalloc( (void**)&outc_d, numgoodV * sizeof(int) ) );
        HANDLE_ERROR( cudaMalloc( (void**)&minisch_d, numgoodV * sizeof(int) ) );
        HANDLE_ERROR( cudaMalloc( (void**)&res_d, ressz * sizeof(int) ) );
        HANDLE_ERROR( cudaMalloc( (void**)&resloc_d, numgoodV * sizeof(int) ) );
        HANDLE_ERROR( cudaMalloc( (void**)&numgoodV_d, 1 * sizeof(int) ) );
        HANDLE_ERROR( cudaMalloc( (void**)&offset_d, 1 * sizeof(int) ) );
        
        HANDLE_ERROR( cudaMemcpy( inc_d, inc+offset, numgoodV * sizeof(int), cudaMemcpyHostToDevice ) );
        HANDLE_ERROR( cudaMemcpy( outc_d, outc+offset, numgoodV * sizeof(int), cudaMemcpyHostToDevice ) );
        HANDLE_ERROR( cudaMemcpy( minisch_d, minisch+offset, numgoodV * sizeof(int), cudaMemcpyHostToDevice ) );
        HANDLE_ERROR( cudaMemcpy( resloc_d, resloc+offset, numgoodV * sizeof(int), cudaMemcpyHostToDevice ) );
        HANDLE_ERROR( cudaMemcpy( res_d, res+resloc[offset],  ressz* sizeof(int), cudaMemcpyHostToDevice ) );
        HANDLE_ERROR( cudaMemcpy( numgoodV_d, &numgoodV, 1 * sizeof(int), cudaMemcpyHostToDevice ) );
        HANDLE_ERROR( cudaMemcpy( offset_d, &resloc_offset, 1 * sizeof(int), cudaMemcpyHostToDevice ) );
        
         
        setup_kernel<<<numBlocks,numThreads>>>(d_state, numgoodV_d);
        update2 <<<numBlocks,numThreads>>> (inc_d, outc_d, minisch_d, res_d, resloc_d, numgoodV_d, offset_d, d_state);
        
        HANDLE_ERROR( cudaMemcpy( res+resloc[offset], res_d,  ressz * sizeof(int), cudaMemcpyDeviceToHost ) );
        
        cudaFree( inc_d );
        cudaFree(d_state);
        cudaFree( outc_d );
        cudaFree( minisch_d );
        cudaFree( res_d );
        cudaFree( resloc_d );
        //cudaFree( mxrptV_d );
        cudaFree( numgoodV_d );
        cudaFree( offset_d );

        remgoodV -= CUDA_MAX_V;
        offset+=CUDA_MAX_V;
    }
}