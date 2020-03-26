#include "ValidPointGroup2.h"

ValidPointGroup2::ValidPointGroup2()
{

}

void ValidPointGroup2::setLeftCenter(const Pixel &p)
{
    leftCenter_ = p;
}

void ValidPointGroup2::setRightCenter(const Pixel &p)
{
    rightCenter_ = p;
}

void ValidPointGroup2::setLeftEdgeI1(const Pixel &p)
{
    leftEdgeI1_ = p;
}

void ValidPointGroup2::setLeftEdgeI2(const Pixel &p)
{
    leftEdgeI2_ = p;
}

void ValidPointGroup2::setRightEdgeI1(const Pixel &p)
{
    rightEdgeI1_ = p;
}

void ValidPointGroup2::setRightEdgeI2(const Pixel &p)
{
    rightEdgeI2_ = p;
}

void ValidPointGroup2::setCenterWorld(const Eigen::Vector3d &p)
{
    centerWorld_ = p;
}

//void ValidPointGroup2::setLeftEdgeI1World(const Eigen::Vector3d &p)
//{
//    leftEdgeI1World_ = p;
//}

//void ValidPointGroup2::setLeftEdgeI2World(const Eigen::Vector3d &p)
//{
//    leftEdgeI2World_ = p;
//}

//void ValidPointGroup2::setRightEdgeI1World(const Eigen::Vector3d &p)
//{
//    rightEdgeI1World_ = p;
//}

//void ValidPointGroup2::setRightEdgeI2World(const Eigen::Vector3d &p)
//{
//    rightEdgeI2World_ = p;
//}

void ValidPointGroup2::setNormalWorld(const Eigen::Vector3d &n)
{
    normalWorld_ = n;
}
