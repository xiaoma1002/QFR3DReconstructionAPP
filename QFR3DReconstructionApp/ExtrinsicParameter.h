#ifndef EXTRINSICPARAMETER_H
#define EXTRINSICPARAMETER_H

#include <QJsonObject>
#include "Eigen/Dense"
//#include "cameraPinHole.h"

class CameraPinHole;

class ExtrinsicParameter
{

public:

    ExtrinsicParameter();

    ExtrinsicParameter(CameraPinHole* cam, const QJsonObject &);

private:

    CameraPinHole *cam_;

    double thetaX_;
    double thetaY_;
    double thetaZ_;

    double deltaX_;
    double deltaY_;
    double deltaZ_;

    Eigen::Vector3d t_;
    Eigen::Matrix3d R_;
    Eigen::Matrix3Xd Rt_;

public:

    inline const Eigen::Vector3d &t() const { return t_;}
    inline const Eigen::Matrix3d &R() const { return R_;}
    inline const Eigen::Matrix3Xd &Rt() const { return Rt_;}

    void print();

    void save();

};

#endif // EXTRINSICPARAMETER_H
