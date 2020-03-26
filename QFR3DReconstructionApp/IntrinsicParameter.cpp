#include "IntrinsicParameter.h"
#include <QDebug>
#include <iostream>
#include "QFRDebugger.h"
#include "cameraPinHole.h"

IntrinsicParameter::IntrinsicParameter()
{

}

IntrinsicParameter::IntrinsicParameter(CameraPinHole *cam, const QJsonObject & dict)
{
    cam_ = cam;

    //- please revist this code @ xiaoma
    //- the units should be in either pixel or meter
    //- we shall discuss if millimter should be inclued
    //- but not for now :)

    focalLengthx_ = dict["FocalLengthx"].toDouble();

    focalLengthy_ = dict["FocalLengthy"].toDouble();

    resolutionU_ = dict["ResolutionU"].toInt();

    resolutionV_ = dict["ResolutionV"].toInt();

    lengthPerPixelx_ = dict["LengthPerPixelx"].toDouble();

    lengthPerPixely_ = dict["LengthPerPixely"].toDouble();

    QString units = dict["units"].toString();

    if (units.compare("meter"))
    {
        lengthPerPixelx_ = 1.0e-3*lengthPerPixelx_;

        lengthPerPixely_ = 1.0e-3*lengthPerPixely_;
    }

    cx_ = 0.5*resolutionU_;

    cy_ = 0.5*resolutionV_;

    K_.row(0) << focalLengthx_, 0.0, cx_;
    K_.row(1) << 0.0, focalLengthy_, cy_;
    K_.row(2) << 0.0, 0.0, 1.0;

    //- focalLengthInMeter calculation should be performed in constructor
    //- declartion of any variable in header does not mean initilization
    //- thus the following piece is moved from .h to the constructor in .cpp
    //- btw, any private variable ends with underscore
    focalLengthInMeter_ = focalLengthx_ * lengthPerPixelx_;

}

//const Eigen::Matrix3d & IntrinsicParameter::K() const
//{
//    return K_;
//}

void IntrinsicParameter::print()
{
//    qDebug() << ".........." << endl;

    qDebug() << "Intrinsic Parameter" << endl;

    qDebug() << "focalLengthx_" << focalLengthx_ << endl;

    qDebug() << "focalLengthy_" << focalLengthy_ << endl;

    qDebug() << "focalLengthInMeter" << focalLengthInMeter_ << endl;

    qDebug() << "resolutionU_" << resolutionU_ << endl;

    qDebug() << "resolutionV_" << resolutionV_ << endl;

    qDebug() << "lengthPerPixelx_" << lengthPerPixelx_ << endl;

    qDebug() << "lengthPerPixely_" << lengthPerPixely_ << endl;

}

void IntrinsicParameter::save()
{
    QFRDebugger dbg;
    dbg.saveFile(K_,cam_->cameraName()+"_K.dat");
}
