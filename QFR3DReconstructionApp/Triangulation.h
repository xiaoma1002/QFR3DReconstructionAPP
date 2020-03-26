#ifndef TRIANGULATION_H
#define TRIANGULATION_H

//#include <QVector3D>
#include "Pixel.h"
#include "Eigen/Dense"

class Triangulation
{

public:

//    Triangulation();

    Triangulation(const Eigen::Matrix3Xd & p1, const Eigen::Matrix3Xd & p2);

private:

    Eigen::Matrix3Xd P1_;

    Eigen::Matrix3Xd P2_;

public:

    Eigen::Vector3d construct3DPoint(const Eigen::Vector3d &p1, const Eigen::Vector3d & p2);

};

#endif // TRIANGULATION_H
