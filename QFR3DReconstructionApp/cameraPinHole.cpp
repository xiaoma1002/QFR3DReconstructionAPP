#include "CameraPinHole.h"
#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include "QFRDebugger.h"

CameraPinHole::CameraPinHole()
{

}

CameraPinHole::CameraPinHole(const int i, const QJsonObject & jsonObject)
{
    jsonObject_ = jsonObject;

    cameraID_ = i;

    cameraName_ = jsonObject["CameraName"].toString();

    const QJsonObject & ipDict = jsonObject["IntrinsicParameter"].toObject();

    intrinsicParameter_ = IntrinsicParameter(this,ipDict);

    const QJsonObject & epDict = jsonObject["ExtrinsicParameter"].toObject();

    extrinsicParameter_ = ExtrinsicParameter(this,epDict);

    imagePath_ = jsonObject["imagePath"].toString();

    calculateP();

    calculatePinHole();

    calculateNormal();

    loadImage();

}

bool CameraPinHole::fileExist(const QString path)
{
    QFileInfo check_file(path);
    // check if file exists and if yes: Is it really a file and no directory?
    if (check_file.exists() && check_file.isFile()) {
        return true;
    } else {
        return false;
    }
}

void CameraPinHole::calculateP()
{
//    Eigen::Matrix3Xd result = Eigen::Matrix3Xd::Zero(3,4);

    P_ = intrinsicParameter_.K()*extrinsicParameter_.Rt();

    QFRDebugger dbg;
    dbg.saveFile(P_, cameraName()+"_P.dat");
}

void CameraPinHole::calculatePinHole()
{
    calculatePinHoleCoordInPixel();
    calculatePinHoleCoordInMeter();
    calculatePinHoleCoordWorld();
}

void CameraPinHole::calculateNormal()
{
    Eigen::Vector3d z;
    z << 0.0,0.0,1.0;

    normal_ = extrinsicParameter_.R()*z;
    double length = normal_.norm();
    normal_ = normal_/length;
}

void CameraPinHole::calculatePinHoleCoordInPixel()
{
    pinHoleCoordInPixel_ << 0.0, 0.0, 0.0;
}

void CameraPinHole::calculatePinHoleCoordInMeter()
{
    pinHoleCoordInMeter_ << 0.0, 0.0, 0.0;
}

void CameraPinHole::calculatePinHoleCoordWorld()
{
    pinHoleCoordWorld_ = R().inverse()*(pinHoleCoordInMeter() - t());
}

const Eigen::Matrix3d &CameraPinHole::K() const
{
    return intrinsicParameter_.K();
}

const Eigen::Matrix3d &CameraPinHole::R() const
{
    return extrinsicParameter_.R();
}

const Eigen::Vector3d &CameraPinHole::t() const
{
    return extrinsicParameter_.t();
}

const int & CameraPinHole::resolutionU() const
{
    return intrinsicParameter_.resolutionU();
}

const int & CameraPinHole::resolutionV() const
{
    return intrinsicParameter_.resolutionV();
}

const double &CameraPinHole::focalLengthInMeter() const
{
    return intrinsicParameter_.focalLengthInMeter();
}

const int &CameraPinHole::focalLengthInPixel() const
{
    return intrinsicParameter_.focalLengthx();
}

const double &CameraPinHole::lengthPerPixel() const
{
    return intrinsicParameter_.lengthPerPixelx();
}

const int &CameraPinHole::cx() const
{
    return intrinsicParameter_.cx();
}

const int &CameraPinHole::cy() const
{
    return intrinsicParameter_.cy();
}

Eigen::Vector3d CameraPinHole::project3DTo2D(const Eigen::Vector3d &p) const
{
    Eigen::Vector4d pH;
    pH(0) = p(0);
    pH(1) = p(1);
    pH(2) = p(2);
    pH(3) = 1.0;

    Eigen::Vector3d x;
    x = P()*pH;

//    std::cout << x << std::endl;

    x(0) = x(0)/x(2);
    x(1) = x(1)/x(2);
    x(2) = x(2)/x(2);

    return x;
}

void CameraPinHole::print()
{
    qDebug() << "Camera " << cameraID_ << " " << cameraName_ << " " << imagePath_ << endl;
    intrinsicParameter_.print();
    extrinsicParameter_.print();
}

void CameraPinHole::save()
{
    intrinsicParameter_.save();
    extrinsicParameter_.save();
}

bool CameraPinHole::loadImage()
{
    if (fileExist(imagePath_))
    {
        image_ = cv::imread(cv::String(imagePath_.toStdString()),cv::IMREAD_GRAYSCALE);

        //- find skeleton of input image

        ThinImage ti(image_);

        skeletonImage_ = ti.skeletonization();

        //- find contour of input image

        ContourDetector cd(image_);

        contourImage_ = cd.findContour();

        //- QFRDebugger qfrd;

        //- qfrd.display(cameraName_, contourImage_);

        return true;
    }
    else
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," Cannot find image file !");
        messageBox.setFixedSize(500,200);
        return false;
    }
}
