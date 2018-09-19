#include "bssneqn_solve.cuh"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include <stdio.h>
#include <iostream>

using namespace std;

__constant__ double ETA_CONST=0.1;
__constant__ double ETA_R0=0.1;
__constant__ double ETA_DAMPING_EXP=0.1;
__constant__ unsigned int lambda[4]={1,2,3,4};
__constant__ double lambda_f[2]={0.8,0.9};

__global__ void cuda_bssn_eqns_points(double * dev_var_in, double * dev_var_out, 
    const unsigned int host_sz_x, const unsigned int host_sz_y, const unsigned int host_sz_z,  
    double pmin_x, double pmin_y, double pmin_z, 
    double hz, double hy, double hx, 
    #include "list_of_offset_para.h"
    ,
    #include "list_of_para.h"
    )
{
    int thread_id = blockIdx.x*256 + threadIdx.x; 

    int i = thread_id%(host_sz_x-6) + 3;
    int j = ((thread_id/(host_sz_x-6))%(host_sz_y-6)) + 3;
    int k = (thread_id/(host_sz_z-6)/(host_sz_y-6)) + 3;

    if (k>=host_sz_z-3) return;

    double z = pmin_z + hz*k;
    double y = pmin_y + hy*j;
    double x = pmin_x + hx*i;

    int pp = i + (host_sz_x)*(j + (host_sz_y)*k);
    double r_coord = sqrt(x*x + y*y + z*z);
    double eta = ETA_CONST;
    if (r_coord >= ETA_R0) {
        eta *= pow( (ETA_R0/r_coord), ETA_DAMPING_EXP);
    }

    #include "cuda_bssneqs.cuh"
}

void calc_bssn_eqns(double * dev_var_in, double * dev_var_out, const unsigned int * sz, const double * pmin, double hz, double hy, double hx, cudaStream_t stream,
#include "list_of_offset_para.h"
, 
#include "list_of_para.h"
)
{
    double pmin_x = pmin[0];
    double pmin_y = pmin[1];
    double pmin_z = pmin[2];

    const unsigned int host_sz_x = sz[0];
    const unsigned int host_sz_y = sz[1];
    const unsigned int host_sz_z = sz[2];

    int total_points = ceil(1.0*(sz[2]-6)*(sz[1]-6)*(sz[0]-6));

    int number_of_blocks = ceil(1.0*total_points/256);

    cuda_bssn_eqns_points<<< number_of_blocks, 256, 0, stream >>>(dev_var_in, dev_var_out, 
        host_sz_x, host_sz_y, host_sz_z, 
        pmin_x, pmin_y, pmin_z, 
        hz, hy, hx, 
        #include "list_of_offset_args.h"
        ,
        #include "list_of_args.h"
    ); 
}
