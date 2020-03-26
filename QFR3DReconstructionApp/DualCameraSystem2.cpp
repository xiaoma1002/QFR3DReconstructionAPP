#include "DualCameraSystem2.h"
#include "QFRDebugger.h"
#include "SurfaceReconstruction2.h"

DualCameraSystem2::DualCameraSystem2(const ValidPointDetector2 &detector):
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
        ValidPointDetector2& validPointDetector = const_cast<ValidPointDetector2&>(validPointDetector_);
        validPointDetector.findValidPoints(F12_);

        QFRDebugger dbg;
        dbg.saveFile(Rrel12_,"Rrel12.dat");
        dbg.saveFile(Rrel21_,"Rrel21.dat");
        dbg.saveFile(Trel12_,"Trel12.dat");
        dbg.saveFile(Trel21_,"Trel21.dat");
        dbg.saveFile(F12_,"F12.dat");
        dbg.saveFile(F21_,"F21.dat");

    }
}

const QList<ValidPointGroup2> &DualCameraSystem2::validPoints() const
{
    return validPointDetector_.validPoints();
}

void DualCameraSystem2::surfaceReconstruction()
{
    SurfaceReconstruction2 surfaceReconstruction(validPointDetector_);
    surfaceReconstruction.write();
}


void DualCameraSystem2::CalculateRrel()
{
    Rrel12_ = camR_.R()*camL_.R().transpose();
    Rrel21_ = camL_.R()*camR_.R().transpose();
}

void DualCameraSystem2::CalculateTrel()
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

void DualCameraSystem2::ConstructFundamentalMatrix()
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
