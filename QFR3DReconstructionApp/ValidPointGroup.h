#ifndef VALIDPOINTGROUP_H
#define VALIDPOINTGROUP_H

#include "Pixel.h"
#include <QVector3D>
#include "Eigen/Dense"

class ValidPointGroup
{

public:

//    ValidPointGroup();

private:

    Pixel leftCenter_;

    Pixel rightCenter_;

    Eigen::Vector3d centerWorld_;

    Pixel leftEdgeI1_;

    Pixel leftEdgeI2_;

    Pixel rightEdgeI1_;

    Pixel rightEdgeI2_;

public:

    void setLeftCenter(const Pixel & p);

    void setRightCenter(const Pixel & p);

    void setCenterWorld(const Eigen::Vector3d & p);

    void setLeftEdgeI1(const Pixel & p);

    void setLeftEdgeI2(const Pixel & p);

    void setRightEdgeI1(const Pixel & p);

    void setRightEdgeI2(const Pixel & p);

    const Pixel & leftCenter() const { return leftCenter_;}

    const Pixel & rightCenter() const { return rightCenter_;}

    const Eigen::Vector3d & centerWorld() const { return centerWorld_;}

    const Pixel & leftEdgeI1() const { return leftEdgeI1_;}

    const Pixel & leftEdgeI2() const { return leftEdgeI2_;}

    const Pixel & rightEdgeI1() const { return rightEdgeI1_;}

    const Pixel & rightEdgeI2() const { return rightEdgeI2_;}
};

#endif // VALIDPOINTGROUP_H
