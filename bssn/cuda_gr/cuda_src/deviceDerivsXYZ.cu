#include "deviceDerivs.cuh"

__device__ void calc_deriv42_x(int tile_size, double * output, double * dev_var_in, const int u_offset, double dx, const unsigned int host_sz_x, const unsigned int host_sz_y, const unsigned int host_sz_z, int bflag)
{
    int tile_x = blockIdx.x%tile_size;
    int tile_y = blockIdx.x/tile_size%tile_size;
    int tile_z = blockIdx.x/tile_size/tile_size;

    int x_offset = tile_x*10;
    int y_offset = tile_y*10;
    int z_offset = tile_z*10;

    int i_thread = threadIdx.x%10;
    int j_thread = threadIdx.x/10%10;
    int k_thread = threadIdx.x/10/10;

    int i = i_thread + x_offset;
    int j = j_thread + y_offset;
    int k = k_thread + z_offset;

    // associated shared memory location
    int i_shared = i_thread + 3;
    int j_shared = j_thread + 3;
    int k_shared = k_thread + 3;

    if (i>=host_sz_x-3) { return; }
    if (i<3) return; 
    if (j>=host_sz_y-1) { return; }
    if (j<1) return; 
    if (k>=host_sz_z-1) { return; }
    if (k<1) return; 
    
    int nx = host_sz_x; 
    int ny = host_sz_y; 

    const double idx = 1.0/dx;
    const double idx_by_12 = idx / 12.0;

    int pp = IDX(i, j, k);
    int loc_pp = k_shared*16*16 + j_shared*16 + i_shared;

    output[pp] = (dev_var_in[loc_pp-2] - 8.0*dev_var_in[loc_pp-1] + 8.0*dev_var_in[loc_pp+1] - dev_var_in[loc_pp+2])*idx_by_12;
}

__device__ void calc_deriv42_y(int tile_size, double* output, double * dev_var_in, const int u_offset, double dy, const unsigned int host_sz_x, const unsigned int host_sz_y, const unsigned int host_sz_z, int bflag)
{
    int tile_x = blockIdx.x%tile_size;
    int tile_y = blockIdx.x/tile_size%tile_size;
    int tile_z = blockIdx.x/tile_size/tile_size;

    int x_offset = tile_x*10;
    int y_offset = tile_y*10;
    int z_offset = tile_z*10;

    int i_thread = threadIdx.x%10;
    int j_thread = threadIdx.x/10%10;
    int k_thread = threadIdx.x/10/10;

    int i = i_thread + x_offset;
    int j = j_thread + y_offset;
    int k = k_thread + z_offset;

    // associated shared memory location
    int i_shared = i_thread + 3;
    int j_shared = j_thread + 3;
    int k_shared = k_thread + 3;

    if (i>=host_sz_x-3) { return; }
    if (i<3) return; 
    if (j>=host_sz_y-3) { return; }
    if (j<3) return; 
    if (k>=host_sz_z-1) { return; }
    if (k<1) return; 

    int nx = host_sz_x; 
    int ny = host_sz_y; 

    const double idy = 1.0/dy;
    const double idy_by_12 = idy / 12.0;
    
    int pp = IDX(i, j, k);
    int loc_pp = k_shared*16*16 + j_shared*16 + i_shared;
    
    output[pp] = (dev_var_in[loc_pp-2*16] - 8.0*dev_var_in[loc_pp-16] + 8.0*dev_var_in[loc_pp+16] - dev_var_in[loc_pp+2*16])*idy_by_12;
}

__device__ void calc_deriv42_z(int tile_size, double* output, double * dev_var_in, const int u_offset, double dz, const unsigned int host_sz_x, const unsigned int host_sz_y, const unsigned int host_sz_z, int bflag)
{
    int tile_x = blockIdx.x%tile_size;
    int tile_y = blockIdx.x/tile_size%tile_size;
    int tile_z = blockIdx.x/tile_size/tile_size;

    int x_offset = tile_x*10;
    int y_offset = tile_y*10;
    int z_offset = tile_z*10;

    int i_thread = threadIdx.x%10;
    int j_thread = threadIdx.x/10%10;
    int k_thread = threadIdx.x/10/10;

    int i = i_thread + x_offset;
    int j = j_thread + y_offset;
    int k = k_thread + z_offset;

    // associated shared memory location
    int i_shared = i_thread + 3;
    int j_shared = j_thread + 3;
    int k_shared = k_thread + 3;

    if (i>=host_sz_x-3) { return; }
    if (i<3) return; 
    if (j>=host_sz_y-3) { return; }
    if (j<3) return; 
    if (k>=host_sz_z-3) { return; }
    if (k<3) return; 

    int nx = host_sz_x; 
    int ny = host_sz_y; 

    const double idz = 1.0/dz;
    const double idz_by_12 = idz / 12.0;

    int pp = IDX(i, j, k);
    int loc_pp = k_shared*16*16 + j_shared*16 + i_shared;

    output[pp] = (dev_var_in[loc_pp-2*16*16] - 8.0*dev_var_in[loc_pp-16*16] + 8.0*dev_var_in[loc_pp+16*16] - dev_var_in[loc_pp+2*16*16])*idz_by_12;
}