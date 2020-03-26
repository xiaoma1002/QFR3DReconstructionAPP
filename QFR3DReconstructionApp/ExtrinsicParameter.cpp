#include "ExtrinsicParameter.h"
#include <QtMath>
#include <QDebug>
#include <iostream>
#include "QFRDebugger.h"
#include "cameraPinHole.h"

ExtrinsicParameter::ExtrinsicParameter()
{

}

ExtrinsicParameter::ExtrinsicParameter(CameraPinHole* cam, const QJsonObject & dict)
{
    cam_ = cam;

    QString units = dict["units"].toString();

    thetaX_ = dict["ThetaX"].toDouble();
    thetaY_ = dict["ThetaY"].toDouble();
    thetaZ_ = dict["ThetaZ"].toDouble();

    if (units.compare("rad"))
    {
        thetaX_ = qDegreesToRadians(thetaX_);
        thetaY_ = qDegreesToRadians(thetaY_);
        thetaZ_ = qDegreesToRadians(thetaZ_);
    }

    deltaX_ = dict["DeltaX"].toDouble();
    deltaY_ = dict["DeltaY"].toDouble();
    deltaZ_ = dict["DeltaZ"].toDouble();

    t_ << deltaX_, deltaY_, deltaZ_;

    Eigen::Matrix3d Rx;

    Rx.row(0) << 1.0, 0.0, 0.0;
    Rx.row(1) << 0.0, qCos(thetaX_), -qSin(thetaX_);
    Rx.row(2) << 0.0, qSin(thetaX_), qCos(thetaX_);

    Eigen::Matrix3d Ry;

    Ry.row(0) << qCos(thetaY_), 0.0, qSin(thetaY_);
    Ry.row(1) << 0.0, 1.0, 0.0;
    Ry.row(2) << -qSin(thetaY_), 0.0, qCos(thetaY_);

    Eigen::Matrix3d Rz;

    Rz.row(0) << qCos(thetaZ_), -qSin(thetaZ_), 0.0;
    Rz.row(1) << qSin(thetaZ_), qCos(thetaZ_), 0.0;
    Rz.row(2) << 0.0, 0.0, 1.0;

    Eigen::Matrix3d RxRyRz;
    RxRyRz = Rx*Ry*Rz;
    R_ = RxRyRz;

    Eigen::Matrix3Xd Rt = Eigen::Matrix3Xd::Zero(3,4);
    Rt.block(0,0,3,3) = R_;
    Rt.block(0,3,3,1) = t_;

    Rt_ = Rt;

}

void ExtrinsicParameter::print()
{
    qDebug() << "Extrinsic Parameter" << endl;

    qDebug() << "thetaX_" <<thetaX_ << endl;

    qDebug() << "thetaY_" <<thetaY_ << endl;

    qDebug() << "thetaZ_" <<thetaZ_ << endl;

    qDebug() << "deltaX_" <<deltaX_ << endl;

    qDebug() << "deltaY_" <<deltaY_ << endl;

    qDebug() << "deltaZ_" <<deltaZ_ << endl;

}

void ExtrinsicParameter::save()
{
    QFRDebugger dbg;
    dbg.saveFile(t_,cam_->cameraName()+"_t.dat");
    dbg.saveFile(R_,cam_->cameraName()+"_R.dat");
    dbg.saveFile(Rt_,cam_->cameraName()+"_Rt.dat");
}
