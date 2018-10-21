//
// Created by milinda on 4/21/17.
/**
*@author Milinda Fernando
*School of Computing, University of Utah
*@brief class definition of the class Block.
*/
//


#include "block.h"

unsigned int m_uiMaxDepth=8;

ot::Block::Block()
{

    m_uiX=0;
    m_uiY=0;
    m_uiZ=0;

    m_uiRegGridLev=0;

    m_uiPaddingWidth=GHOST_WIDTH;

    m_uiEleOrder=0;
    m_uiSize1D=0;

    m_uiSzX=m_uiSize1D;
    m_uiSzY=m_uiSize1D;
    m_uiSzZ=m_uiSize1D;

    m_uiBlkElem_1D=1u<<(m_uiRegGridLev);



}


ot::Block::Block(unsigned int px,unsigned int py, unsigned int pz, unsigned int regLev,unsigned int eleOrder)
{

    m_uiX=px;
    m_uiY=py;
    m_uiZ=pz;

    m_uiRegGridLev=regLev;

    m_uiPaddingWidth=GHOST_WIDTH;

    m_uiEleOrder=eleOrder;
    m_uiSize1D=m_uiEleOrder*((1u<<(m_uiRegGridLev))-1)+(m_uiEleOrder+1)+2*m_uiPaddingWidth;

    m_uiSzX=m_uiSize1D;
    m_uiSzY=m_uiSize1D;
    m_uiSzZ=m_uiSize1D;

    m_uiBlkElem_1D=1u<<(m_uiRegGridLev);



}


ot::Block::~Block()
{

}


double ot::Block::computeGridDx() const
{
    return DX;
}

double ot::Block::computeGridDy() const
{
    return DY;
}

double ot::Block::computeGridDz() const
{
    return DZ;
}



void ot::Block::setOffset(DendroIntL offset)
{
    m_uiOffset=offset;
}

double ot::Block::computeDx(const Point & d_min,const Point & d_max) const
{
    return ((d_max.x()-d_min.x())/((double)(1u<<m_uiMaxDepth)))*(computeGridDx());
}

double ot::Block::computeDy(const Point & d_min,const Point & d_max) const
{
    return ((d_max.y()-d_min.y())/((double)(1u<<m_uiMaxDepth)))*(computeGridDy());
}

double ot::Block::computeDz(const Point & d_min,const Point & d_max) const
{
    return ((d_max.z()-d_min.z())/((double)(1u<<m_uiMaxDepth)))*(computeGridDz());
}