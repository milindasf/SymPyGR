//
// Created by milinda on 8/30/18.
//

#include "profile_gpu.h"

namespace cuda
{
    namespace profile
    {

        /** overall gpu exuxution time*/
        bssn_profiler_t t_overall;

        /**host to device communication*/
        bssn_profiler_t t_H2D_Comm;
        /**device to host communication*/
        bssn_profiler_t t_D2H_Comm;

        /**memory allocation deallocation time*/
        bssn_profiler_t t_cudaMalloc_derivs;

       
        /**total rhs computation time*/
        bssn_profiler_t t_rhs_gpu;
        bssn_profiler_t t_rhs_cpu;
        bssn_profiler_t t_rhs_total;





    } // end of namespace profile
} // end of namespace cuda


void cuda::profile::initialize()
{
   t_overall.start();
    t_H2D_Comm.start();
    t_D2H_Comm.start();
    t_cudaMalloc_derivs.start();
    t_rhs_total.start();
    t_rhs_cpu.start();
    t_rhs_gpu.start();

}




void cuda::profile::printOutput(const std::vector<ot::Block>& localBlkList) {


    /*
       * deriv    |    block   |            pp            |   total               |
       * dx       |       3    |      4 mul + 3 add =7    |    24+35+46 = 105     |
       * dxx      |       2    |      6 mul + 4 add =10   |    11*3 =33    |
       * adx      |       3    |      5 mul + 4 add =9    |    72     |
       * kox      |      11    |      8 mul + 6 add =16   |    72     |
       *
       * total pp =2865
       * total block =19
       *
       *
       * */


    /* var              original                after CSE
     * a_rhs               12                      12
     * b_rhs               51                      39
     * gt_rhs             210                     162
     * chi_rhs             22                      20
     * At_rhs          630012                    3569
     * K_rhs             3960                     501
     * Gt_rhs           16710                     732
     * B_rhs            17226                     765
     *
     * total            668203	                 5800

     * */



    // floating point operation computed once per block
    const unsigned int NUM_DERIV_OPS_BLOCK=19;
    // floating point operation computed for each pp
    const unsigned int NUM_DERIV_OPS_PP=2865;

    const unsigned int NUM_RHS_OPS_PP=5800+24*4;

    unsigned long int unzipInternal=0;
    unsigned long int unzipTotal=0;

    const char separator    = ' ';
    const int nameWidth     = 60;
    const int numWidth      = 10;

    for(unsigned int blk=0;blk<localBlkList.size();blk++)
    {
        unzipInternal+=(localBlkList[blk].get1DArraySize()-2*localBlkList[blk].get1DPadWidth())*(localBlkList[blk].get1DArraySize()-2*localBlkList[blk].get1DPadWidth())*(localBlkList[blk].get1DArraySize()-2*localBlkList[blk].get1DPadWidth());
        unzipTotal+=(localBlkList[blk].get1DArraySize()*localBlkList[blk].get1DArraySize()*localBlkList[blk].get1DArraySize());
    }

    std::cout<<"unzip dof: "<<unzipTotal<<std::endl;
    std::cout<<"unzip internal: "<<unzipInternal<<std::endl;

    double t_stat;
    t_stat=t_overall.seconds;
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"+cuda_overall(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;;

    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -deriv(compute)(flops) overal: ";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<(((unzipInternal*NUM_DERIV_OPS_PP)+localBlkList.size()*NUM_DERIV_OPS_BLOCK)/t_stat)<<std::endl;

    t_stat=t_H2D_Comm.seconds;
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -H2D(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;

    t_stat=t_D2H_Comm.seconds;
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -D2H(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;


    t_stat=t_cudaMalloc_derivs.seconds;
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -deriv(malloc)(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;

    /*t_stat=t_derivs.seconds;
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -deriv(compute)(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;

    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -deriv(compute)(flops)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<(((unzipInternal*NUM_DERIV_OPS_PP)+localBlkList.size()*NUM_DERIV_OPS_BLOCK)/t_stat)<<std::endl;*/

    t_stat=t_rhs_cpu.seconds;
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -rhs_cpu(compute)(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;


    t_stat=(t_rhs_total.seconds-t_rhs_cpu.seconds);
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -rhs_gpu(compute)(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;

    t_stat=t_rhs_total.seconds;
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -rhs_total(compute)(s)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<t_stat<<std::endl;

    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator) <<"  -overall(flops)";
    std::cout << std::left << std::setw(nameWidth) << std::setfill(separator)<<(((unzipInternal*(NUM_DERIV_OPS_PP+NUM_RHS_OPS_PP))+localBlkList.size()*NUM_DERIV_OPS_BLOCK)/t_stat)<<std::endl;

    return;



}


