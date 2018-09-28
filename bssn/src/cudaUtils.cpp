//
// Created by milinda on 8/9/18.
//
/**
 * @author Milinda Fernando
 * School of Computing, University of Utah
 * @brief Contains utility function for the host related to GPUs
 * */
#include "cudaUtils.h"

namespace cuda
{


    cudaDeviceProp* getGPUDeviceInfo(unsigned int device)
    {

        cudaDeviceProp deviceProp;
        cudaGetDeviceProperties(&deviceProp,device);

        cudaDeviceProp* __deviceProp;
        cudaMalloc(&__deviceProp,sizeof(cudaDeviceProp));
        CUDA_CHECK_ERROR();

        cudaMemcpy(__deviceProp,&deviceProp,sizeof(cudaDeviceProp),cudaMemcpyHostToDevice);
        CUDA_CHECK_ERROR();

        return __deviceProp;

    }



    void computeDendroBlockToGPUMap(const ot::Block* blkList, unsigned int numBlocks, std::vector<unsigned int >& blockMap,dim3 & gridDim)
    {
        std::vector<unsigned int> dendroBlockToGPUBlockMap;
        dendroBlockToGPUBlockMap.resize(numBlocks);

        unsigned int DATA_BLOCK_GROUP_THRESHOLD_1D=32;
        unsigned int blkSzCount=0;
        unsigned int gpuBlockId=0;
        for(unsigned int blk=0;blk<numBlocks;++blk)
        {
            blkSzCount+=blkList[blk].get1DArraySize();
            dendroBlockToGPUBlockMap[blk]=gpuBlockId;

            if(blkSzCount>DATA_BLOCK_GROUP_THRESHOLD_1D)
            {
                blkSzCount=0;
                gpuBlockId++;
            }

        }

        const unsigned int TOTAL_GPU_BLOCKS=dendroBlockToGPUBlockMap.back()+1;
        gridDim=dim3(TOTAL_GPU_BLOCKS,1,1);

        blockMap.clear();

        blockMap.resize(TOTAL_GPU_BLOCKS*2);
        gpuBlockId=0;

        unsigned int blkCount=0;
        while(blkCount<dendroBlockToGPUBlockMap.size())
        {
            //std::cout<<"gpublock ID: "<<gpuBlockId<<" blkCount: "<<blkCount<<std::endl;
            blockMap[2*gpuBlockId]=blkCount;
            if(blkCount<(dendroBlockToGPUBlockMap.size()-1) && dendroBlockToGPUBlockMap[blkCount]==dendroBlockToGPUBlockMap[blkCount+1])
            {
                blkCount++;
                while(blkCount<(dendroBlockToGPUBlockMap.size()-1) && dendroBlockToGPUBlockMap[blkCount]==dendroBlockToGPUBlockMap[blkCount+1]) blkCount++;
                blockMap[2*gpuBlockId+1]=blkCount+1;

            }else {
                blockMap[2*gpuBlockId+1]=blkCount+1;
            }
            blkCount++;
            gpuBlockId++;

        }

        /*for(unsigned int blk=0;blk<blkList.size();++blk)
            std::cout<<"blk: "<<blk<<" gpu bid : "<<dendroBlockToGPUBlockMap[blk]<<" block size: "<<blkList[blk].get1DArraySize()<<std::endl;

        for(unsigned int bid=0;bid<TOTAL_GPU_BLOCKS;bid++)
            std::cout<<"block id : "<<bid<<" begin: "<<inverseDendroBlockToGPUBlockMap[2*bid]<<" end : "<<inverseDendroBlockToGPUBlockMap[2*bid+1]<<std::endl;*/

        return;

    }




}