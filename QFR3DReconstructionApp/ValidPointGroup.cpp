#include "ValidPointGroup.h"

//ValidPointGroup::ValidPointGroup()
//{

//}

void ValidPointGroup::setLeftCenter(const Pixel &p)
{
    leftCenter_ = p;
}

void ValidPointGroup::setRightCenter(const Pixel &p)
{
    rightCenter_ = p;
}

void ValidPointGroup::setCenterWorld(const Eigen::Vector3d &p)
{
    centerWorld_ = p;
}

void ValidPointGroup::setLeftEdgeI1(const Pixel &p)
{
    leftEdgeI1_ = p;
}

void ValidPointGroup::setLeftEdgeI2(const Pixel &p)
{
    leftEdgeI2_ = p;
}

void ValidPointGroup::setRightEdgeI1(const Pixel &p)
{
    rightEdgeI1_ = p;
}

void ValidPointGroup::setRightEdgeI2(const Pixel &p)
{
    rightEdgeI2_ = p;
}
