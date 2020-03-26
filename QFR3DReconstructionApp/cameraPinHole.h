#ifndef CAMERAPINHOLE_H
#define CAMERAPINHOLE_H


#include <QJsonObject>

#include "IntrinsicParameter.h"
#include "ExtrinsicParameter.h"
#include "opencv2/opencv.hpp"
#include "ThinImage.h"
#include "ContourDetector.h"
#include "Eigen/Dense"

class CameraPinHole
{

public:

    CameraPinHole();

    CameraPinHole(const int ith, const QJsonObject&);

private:

    QJsonObject jsonObject_;

    int cameraID_;

    QString cameraName_;

    IntrinsicParameter intrinsicParameter_;

    ExtrinsicParameter extrinsicParameter_;

    QString imagePath_;

    cv::Mat image_;

    cv::Mat skeletonImage_;

    cv::Mat contourImage_;

    bool fileExist(const QString path);

    Eigen::Matrix3Xd P_;

    Eigen::Vector3d normal_;

    void calculateP();

    Eigen::Vector3d pinHoleCoordInPixel_;

    Eigen::Vector3d pinHoleCoordInMeter_;

    void calculatePinHoleCoordInPixel();

    void calculatePinHoleCoordInMeter();

    //- pinhole in world coordinate system
    Eigen::Vector3d pinHoleCoordWorld_;

    void calculatePinHoleCoordWorld();

    void calculatePinHole();

    void calculateNormal();

public:

    const Eigen::Matrix3d & K() const;

    const Eigen::Matrix3d & R() const;

    const Eigen::Vector3d & t() const;

    inline const Eigen::Matrix3Xd & P() const { return P_; }

    const int & resolutionU() const;

    const int & resolutionV() const;

    const double & focalLengthInMeter() const;
    
    const int & focalLengthInPixel() const;

    const double & lengthPerPixel() const;

    const int &cx() const;

    const int &cy() const;

    inline const cv::Mat & skeletonImage() const { return skeletonImage_; }

    inline const cv::Mat & contourImage() const { return contourImage_; }

    inline const QString & cameraName() const { return cameraName_; }

    inline const QString & imagePath() const { return imagePath_; }

    inline const Eigen::Vector3d & pinHoleCoordInPixel() const { return pinHoleCoordInPixel_; }

    inline const Eigen::Vector3d & pinHoleCoordInMeter() const { return pinHoleCoordInMeter_; }

    inline const Eigen::Vector3d & pinHoleCoordWorld() const { return pinHoleCoordWorld_; }

    inline const Eigen::Vector3d & normal() const { return normal_; }

    //- Project 3D to 2D

    Eigen::Vector3d project3DTo2D(const Eigen::Vector3d&p) const;

    void print();

    void save();

    bool loadImage();
};

#endif // CAMERAPINHOLE_H
