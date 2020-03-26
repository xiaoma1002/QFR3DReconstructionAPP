#ifndef INTRINSICPARAMETER_H
#define INTRINSICPARAMETER_H

//#include "matrixdeclarations.h"
#include <QJsonObject>
#include "Eigen/Dense"
//#include "cameraPinHole.h"

class CameraPinHole;

class IntrinsicParameter
{

public:

    IntrinsicParameter();

    IntrinsicParameter(CameraPinHole *, const QJsonObject&);

private:

    CameraPinHole *cam_;

    int focalLengthx_;

    int focalLengthy_;

    int resolutionU_;

    int resolutionV_;

    //- unit : meter/multimeter
    double lengthPerPixelx_;

    double lengthPerPixely_;

    //- unit : meter
    double focalLengthInMeter_;

    Eigen::Matrix3d K_;

    int cx_;

    int cy_;

public:

    inline const Eigen::Matrix3d & K() const { return K_; }

    inline const int &focalLengthx() const { return focalLengthx_; }

    inline const int &focalLengthy() const { return focalLengthy_; }

    inline const int &resolutionU() const { return resolutionU_; }

    inline const int &resolutionV() const { return resolutionV_; }

    inline const double &focalLengthInMeter() const { return focalLengthInMeter_; }

    inline const double &lengthPerPixelx() const { return lengthPerPixelx_; }

    inline const double &lengthPerPixely() const { return lengthPerPixely_; }

    inline const int & cx() const { return cx_; }

    inline const int & cy() const { return cy_; }

    void print();

    void save();

};

#endif // INTRINSICPARAMETER_H
