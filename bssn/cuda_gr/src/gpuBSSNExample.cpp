//
// Created by milinda on 8/20/18.
//


#include "gpuBSSNExample.h"


int main (int argc, char** argv)
{
    if(argc<2)
        std::cout<<"Usage: "<<argv[0]<<" low_level high_level numBlocks"<<std::endl;


    const unsigned int low_level=atoi(argv[1]);
    const unsigned int high_level=atoi(argv[2]);
    const unsigned int numBlocks=atoi(argv[3]);

    const double mean = 0.5*(high_level-low_level);
    const double sd=(high_level-mean);

    const unsigned int MAX_STARS=50;
    const unsigned int eleOrder=4;

    std::default_random_engine generator;
    std::normal_distribution<double> distribution(mean,sd);

    const unsigned int ALIGNMENT_FACTOR=16;

    unsigned int* blkLevs=new unsigned int[numBlocks];
    unsigned int count=0;
    while(count<numBlocks){
        double number = distribution(generator);
        if ((number>=low_level)&&(number<high_level))
        {
            blkLevs[count]=(int)number;
            count++;
        }


    }

    std::vector<ot::Block> blkList;
    blkList.resize(numBlocks);

    unsigned int unzipSz=0;

    for(unsigned int i=0;i<numBlocks;i++)
    {
        blkList[i]=ot::Block(2,2,2,blkLevs[i],eleOrder);
        blkList[i].setBlkNodeFlag(0);
        blkList[i].setOffset(unzipSz);
        unzipSz+=(std::ceil((blkList[i].getAllocationSzX()*blkList[i].getAllocationSzY()*blkList[i].getAllocationSzZ())/(double)ALIGNMENT_FACTOR)*ALIGNMENT_FACTOR);
    }

    delete [] blkLevs;

    const unsigned int UNZIP_DOF=unzipSz;


    // variable input
    double ** varUnzipIn = new double*[bssn::BSSN_NUM_VARS];
    double ** varUnzipOutGPU = new double*[bssn::BSSN_NUM_VARS];
    double ** varUnzipOutCPU = new double*[bssn::BSSN_NUM_VARS];


    for(unsigned int var=0;var<bssn::BSSN_NUM_VARS;var++)
    {
        varUnzipIn[var]=new double[UNZIP_DOF];
        varUnzipOutGPU[var]=new double[UNZIP_DOF];
        varUnzipOutCPU[var]=new double[UNZIP_DOF];
    }


    double u_var[bssn::BSSN_NUM_VARS];

    std::cout<<YLW<<" ================================"<<NRM<<std::endl;
    std::cout<<YLW<<"     data init begin             "<<NRM<<std::endl;
    std::cout<<YLW<<" ================================"<<NRM<<std::endl;

    // initialize the input data
    unsigned int offset,bflag;
    unsigned int sz[3];
    double dx,dy,dz;
    double x,y,z;
    const double ptmin[3]={1.0,1.0,1.0};
    double ptmax[3];
    for(unsigned int blk=0;blk<blkList.size();blk++)
    {
        offset=blkList[blk].getOffset();
        sz[0]=blkList[blk].getAllocationSzX();
        sz[1]=blkList[blk].getAllocationSzY();
        sz[2]=blkList[blk].getAllocationSzZ();

        dx=blkList[blk].computeGridDx();
        dy=blkList[blk].computeGridDy();
        dz=blkList[blk].computeGridDz();

        ptmax[0]=ptmin[0]+dx*(sz[0]);
        ptmax[1]=ptmin[1]+dx*(sz[1]);
        ptmax[2]=ptmin[2]+dx*(sz[2]);

        bflag=blkList[blk].getBlkNodeFlag();

        for (unsigned int k = 0; k < sz[2]; k++) {
           z = ptmin[2] + k*dz;

            for (unsigned int j = 0; j < sz[1]; j++) {
                y = ptmin[1] + j*dy;

                for (unsigned int i = 0; i < sz[0]; i++) {
                    x = ptmin[0] + i*dx;
                    bssn::fake_initial_data(x,y,z,u_var);
                    for(unsigned int var=0;var<bssn::BSSN_NUM_VARS;var++)
                        varUnzipIn[var][offset+k*(sz[1]*sz[0])+j*(sz[0])+i]=u_var[var];
                }
            }
        }

    }

    std::cout<<YLW<<" ================================"<<NRM<<std::endl;
    std::cout<<YLW<<"     data init end             "<<NRM<<std::endl;
    std::cout<<YLW<<" ================================"<<NRM<<std::endl;

    std::cout<<"\n\n"<<std::endl;


    std::cout<<YLW<<" ================================"<<NRM<<std::endl;
    std::cout<<YLW<<"     CPU begin             "<<NRM<<std::endl;
    std::cout<<YLW<<" ================================"<<NRM<<std::endl;

    bssn::timer::initialize();

    bssn::timer::total_runtime.start();


    for(unsigned int blk=0;blk<blkList.size();blk++) {
        offset = blkList[blk].getOffset();
        sz[0] = blkList[blk].getAllocationSzX();
        sz[1] = blkList[blk].getAllocationSzY();
        sz[2] = blkList[blk].getAllocationSzZ();

        dx = blkList[blk].computeGridDx();
        dy = blkList[blk].computeGridDy();
        dz = blkList[blk].computeGridDz();

        bflag=blkList[blk].getBlkNodeFlag();

        ptmax[0] = ptmin[0] + dx * (sz[0]);
        ptmax[1] = ptmin[1] + dx * (sz[1]);
        ptmax[2] = ptmin[2] + dx * (sz[2]);

        bssnrhs_sep(varUnzipOutCPU, (const double **)varUnzipIn, offset, ptmin, ptmax, sz, bflag);

    }

    bssn::timer::total_runtime.stop();

    std::cout<<YLW<<" ================================"<<NRM<<std::endl;
    std::cout<<YLW<<"     CPU end             "<<NRM<<std::endl;
    std::cout<<YLW<<" ================================"<<NRM<<std::endl;

    bssn::timer::profileInfo();


#ifdef BSSN_ENABLE_CUDA


    std::cout<<YLW<<" ================================"<<NRM<<std::endl;
    std::cout<<YLW<<"     GPU begin             "<<NRM<<std::endl;
    std::cout<<YLW<<" ================================"<<NRM<<std::endl;

    cuda::BSSNComputeParams bssnParams;
    bssnParams.BSSN_LAMBDA[0]=bssn::BSSN_LAMBDA[0];
    bssnParams.BSSN_LAMBDA[1]=bssn::BSSN_LAMBDA[1];
    bssnParams.BSSN_LAMBDA[2]=bssn::BSSN_LAMBDA[2];
    bssnParams.BSSN_LAMBDA[3]=bssn::BSSN_LAMBDA[3];

    bssnParams.BSSN_LAMBDA_F[0]=bssn::BSSN_LAMBDA_F[0];
    bssnParams.BSSN_LAMBDA_F[1]=bssn::BSSN_LAMBDA_F[1];

    bssnParams.BSSN_ETA_POWER[0]=bssn::BSSN_ETA_POWER[0];
    bssnParams.BSSN_ETA_POWER[1]=bssn::BSSN_ETA_POWER[1];

    bssnParams.BSSN_ETA_R0=bssn::ETA_R0;
    bssnParams.ETA_CONST=bssn::ETA_CONST;
    bssnParams.ETA_DAMPING=bssn::ETA_DAMPING;
    bssnParams.ETA_DAMPING_EXP=bssn::ETA_DAMPING_EXP;
    bssnParams.KO_DISS_SIGMA=bssn::KO_DISS_SIGMA;
    std::vector<cuda::_Block> blkCudaList;
    blkCudaList.resize(blkList.size());
    double hx[3];

    for(unsigned int blk=0;blk<blkList.size();blk++)
    {
        offset=blkList[blk].getOffset();
        sz[0]=blkList[blk].getAllocationSzX();
        sz[1]=blkList[blk].getAllocationSzY();
        sz[2]=blkList[blk].getAllocationSzZ();

        bflag=blkList[blk].getBlkNodeFlag();

        dx = blkList[blk].computeGridDx();
        dy = blkList[blk].computeGridDy();
        dz = blkList[blk].computeGridDz();

        hx[0]=dx;
        hx[1]=dy;
        hx[2]=dz;

        ptmax[0] = ptmin[0] + dx * (sz[0]);
        ptmax[1] = ptmin[1] + dx * (sz[1]);
        ptmax[2] = ptmin[2] + dx * (sz[2]);

        blkCudaList[blk]=cuda::_Block((const double *)ptmin,(const double *)ptmax,offset,bflag,(const unsigned int*)sz, (const double *)hx);

    }

    cuda::profile::initialize();

    cuda::computeRHS(varUnzipOutGPU,(const double **)varUnzipIn,&(*(blkCudaList.begin())),blkCudaList.size(),(const cuda::BSSNComputeParams*) &bssnParams);

    cuda::profile::printOutput(blkList);

    std::cout<<YLW<<" ================================"<<NRM<<std::endl;
    std::cout<<YLW<<"     GPU end             "<<NRM<<std::endl;
    std::cout<<YLW<<" ================================"<<NRM<<std::endl;

#endif


    double threshold = 1e-10;
    for(int bssn_var=0; bssn_var<bssn::BSSN_NUM_VARS; bssn_var++){
        for (int point=0; point<UNZIP_DOF; point++){
            double diff = varUnzipOutGPU[bssn_var][point] - varUnzipOutCPU[bssn_var][point];
            if (fabs(diff)>threshold){
                printf("ERROR \t GPU:%f \t CPU:%f \t Diff:%f\n", varUnzipOutGPU[bssn_var][point], varUnzipOutCPU[bssn_var][point], diff);
                exit(0);
            }
        }
    }



    for(unsigned int var=0;var<bssn::BSSN_NUM_VARS;var++)
    {
        delete [] varUnzipIn[var];
        delete [] varUnzipOutCPU[var];
        delete [] varUnzipOutGPU[var];
    }


    delete [] varUnzipIn;
    delete [] varUnzipOutCPU;
    delete [] varUnzipOutGPU;



    return 0;


}



