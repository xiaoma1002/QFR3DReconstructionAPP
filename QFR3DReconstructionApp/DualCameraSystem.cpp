#include "DualCameraSystem.h"
#include "SurfaceReconstruction.h"
#include <QDebug>
#include <iostream>
#include "QFRDebugger.h"

DualCameraSystem::DualCameraSystem(const ValidPointDetector &detector):
    validPointDetector_(detector),
    camL_(detector.camL()),
    camR_(detector.camR())
{
    //- cast const camera to non-const camera
    //- for reloading images

    CameraPinHole& camL = const_cast<CameraPinHole&>(camL_);
    CameraPinHole& camR = const_cast<CameraPinHole&>(camR_);

    bool loadedL = camL.loadImage();

    bool loadedR = camR.loadImage();

    if (loadedL && loadedR) {

        CalculateRrel();

        CalculateTrel();

        ConstructFundamentalMatrix();

        //        validPointDetector_ = ValidPointDetector(camL,camR,F_);
        ValidPointDetector& validPointDetector = const_cast<ValidPointDetector&>(validPointDetector_);
        validPointDetector.findValidPoints(F12_,F21_);

        QFRDebugger dbg;
        dbg.saveFile(Rrel12_,"Rrel12.dat");
        dbg.saveFile(Rrel21_,"Rrel21.dat");
        dbg.saveFile(Trel12_,"Trel12.dat");
        dbg.saveFile(Trel21_,"Trel21.dat");
        dbg.saveFile(F12_,"F12.dat");
        dbg.saveFile(F21_,"F21.dat");

    }
}

void DualCameraSystem::CalculateRrel()
{
    Rrel12_ = camR_.R()*camL_.R().transpose();
    Rrel21_ = camL_.R()*camR_.R().transpose();
}

void DualCameraSystem::CalculateTrel()
{
    Trel12_ = camR_.t() - Rrel12_*camL_.t();
    Trel21_ = camL_.t() - Rrel21_*camR_.t();
}

//# compute F matrix
//# F = K_2^-T S_t R K_1^-1
//t1 = Trel[0][0]
//t2 = Trel[1][0]
//t3 = Trel[2][0]

//St = np.array([[0,-t3,t2],
//              [t3,0,-t1],
//              [-t2,t1,0]])

void DualCameraSystem::ConstructFundamentalMatrix()
{
    {
        Eigen::Matrix3d St12;

        double t1 = Trel12_(0);
        double t2 = Trel12_(1);
        double t3 = Trel12_(2);

        St12.row(0) << 0.0,-t3,t2;
        St12.row(1) << t3,0.0,-t1;
        St12.row(2) << -t2,t1,0.0;

        Eigen::Matrix3d K1 = camL_.K();
        Eigen::Matrix3d K2 = camR_.K();

        Eigen::Matrix3d invK1 = K1.inverse();

        Eigen::Matrix3d invK2 = K2.inverse();

        //- only valid at image coordinate system
        //- thus, pixelUV coordinates should be converted
        //- to image coordinates before searching for
        //- cooresponding point in the other image

        F12_ = invK2.transpose()*St12;

        F12_ = F12_*Rrel12_;

        F12_ = F12_*invK1;
    }
    {
        Eigen::Matrix3d St21;

        double t1 = Trel21_(0);
        double t2 = Trel21_(1);
        double t3 = Trel21_(2);

        St21.row(0) << 0.0,-t3,t2;
        St21.row(1) << t3,0.0,-t1;
        St21.row(2) << -t2,t1,0.0;

        Eigen::Matrix3d K1 = camR_.K();
        Eigen::Matrix3d K2 = camL_.K();

        Eigen::Matrix3d invK1 = K1.inverse();

        Eigen::Matrix3d invK2 = K2.inverse();

        //- only valid at image coordinate system
        //- thus, pixelUV coordinates should be converted
        //- to image coordinates before searching for
        //- cooresponding point in the other image

        F21_ = invK2.transpose()*St21;

        F21_ = F21_*Rrel21_;

        F21_ = F21_*invK1;
    }
}

const QList<ValidPointGroup> &DualCameraSystem::validPointsL() const
{
    return validPointDetector_.validPointsL();
}

const QList<ValidPointGroup> &DualCameraSystem::validPointsR() const
{
    return validPointDetector_.validPointsR();
}

void DualCameraSystem::surfaceReconstruction()
{
    //- all valid points
    //-const QList<ValidPointGroup> &vpgl = validPoints();

    //- location of pinhole
    Eigen::Vector3d HL = Eigen::Vector3d::Zero();
    Eigen::Vector3d HR = Eigen::Vector3d::Zero();

    HL(2) = camL_.focalLengthInMeter();
    HR(2) = camR_.focalLengthInMeter();

    QFRDebugger dbg;
    dbg.saveFile(HL,camL_.cameraName()+"_based_H.dat");
    dbg.saveFile(HR,camR_.cameraName()+"_based_H.dat");

    SurfaceReconstruction sr(HL,HR,validPointDetector_);

    sr.construct();

}
