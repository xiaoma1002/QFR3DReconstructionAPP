#ifndef VALIDPOINTGROUP2_H
#define VALIDPOINTGROUP2_H

#include "Pixel.h"
#include "Eigen/Dense"

class ValidPointGroup2
{

public:

    ValidPointGroup2();

private:

    Pixel leftCenter_;

    Pixel rightCenter_;

    Pixel leftEdgeI1_;

    Pixel leftEdgeI2_;

    Pixel rightEdgeI1_;

    Pixel rightEdgeI2_;

    Eigen::Vector3d centerWorld_;

//    Eigen::Vector3d leftEdgeI1World_;

//    Eigen::Vector3d leftEdgeI2World_;

//    Eigen::Vector3d rightEdgeI1World_;

//    Eigen::Vector3d rightEdgeI2World_;

    Eigen::Vector3d normalWorld_;

public:

    void setLeftCenter(const Pixel & p);
    void setRightCenter(const Pixel & p);
    void setLeftEdgeI1(const Pixel & p);
    void setLeftEdgeI2(const Pixel & p);
    void setRightEdgeI1(const Pixel & p);
    void setRightEdgeI2(const Pixel & p);

    void setCenterWorld(const Eigen::Vector3d & p);
//    void setLeftEdgeI1World(const Eigen::Vector3d & p);
//    void setLeftEdgeI2World(const Eigen::Vector3d & p);
//    void setRightEdgeI1World(const Eigen::Vector3d & p);
//    void setRightEdgeI2World(const Eigen::Vector3d & p);

    void setNormalWorld(const Eigen::Vector3d &n);

    inline const Pixel& leftCenter() const { return leftCenter_; }
    inline const Pixel& rightCenter() const { return rightCenter_; }
    inline const Pixel& leftEdgeI1() const { return leftEdgeI1_; }
    inline const Pixel& leftEdgeI2() const { return leftEdgeI2_; }
    inline const Pixel& rightEdgeI1() const { return rightEdgeI1_; }
    inline const Pixel& rightEdgeI2() const { return rightEdgeI2_; }

    inline const Eigen::Vector3d &centerWorld() const { return centerWorld_; }
//    inline const Eigen::Vector3d &leftEdgeI1World() const { return leftEdgeI1World_; }
//    inline const Eigen::Vector3d &leftEdgeI2World() const { return leftEdgeI2World_; }
//    inline const Eigen::Vector3d &rightEdgeI1World() const { return rightEdgeI1World_; }
//    inline const Eigen::Vector3d &rightEdgeI2World() const { return rightEdgeI2World_; }

    inline const Eigen::Vector3d &normalWorld() const { return normalWorld_; }
};

#endif // VALIDPOINTGROUP2_H
