#include "SurfaceReconstruction.h"
#include "PixelUVImageXYTransformer.h"
#include "Eigen/Dense"

#include <QDebug>
#include <QTextStream>
#include <QFRDebugger.h>

//#include "Eigen/Core"

//SurfaceReconstruction::SurfaceReconstruction()
//{

//}

SurfaceReconstruction::SurfaceReconstruction(\
        const Eigen::Vector3d &HL, \
        const Eigen::Vector3d &HR, \
        const ValidPointDetector &detector):
    HL_(HL),
    HR_(HR),
    detector_(detector)
{}

void SurfaceReconstruction::construct()
{
    const CameraPinHole & camL = detector_.camL();
    const CameraPinHole & camR = detector_.camR();

    const QList<ValidPointGroup> & vpglL = detector_.validPointsL();
    const QList<ValidPointGroup> & vpglR = detector_.validPointsR();

    construct3DPoints(camL,vpglL);
    construct3DPoints(camR,vpglR);
}

void SurfaceReconstruction::construct3DPoints(const CameraPinHole &camera, const QList<ValidPointGroup> &vpgl)
{
    const Eigen::Vector3d & H = HL_;

    //const Eigen::Matrix3Xd & P = cam.P();

    int uMax = camera.resolutionU();
    int vMax = camera.resolutionV();

    //Eigen::Vector3d T = cam.t();

    double lengthPerPixel = camera.lengthPerPixel();

    //- inverse of R1
    Eigen::Matrix3d Rinv = camera.R().inverse();

    //- camera translation vector in relative to world's origin
    const Eigen::Vector3d &T = camera.t();

    int size = vpgl.size();

    const Eigen::Matrix3Xd &P = camera.P();


    PixelUVImageXYTransformer coordTransformer(uMax,vMax);

    QString edgeFilename,centerlineFilename;
    edgeFilename = camera.cameraName() + "_Edge.obj";

    centerlineFilename = camera.cameraName() + "_Centerline.obj";

    QString debuggerFilename(camera.cameraName()+"-debugger.dat");

    QFile edgeFile( edgeFilename );
    QFile centerlineFile(centerlineFilename);
    edgeFile.open(QIODevice::WriteOnly);
    centerlineFile.open(QIODevice::WriteOnly);
    QFile debuggerFile(debuggerFilename);
    debuggerFile.open(QIODevice::WriteOnly);

    QTextStream edgeStream( &edgeFile );
    QTextStream centerlineStream( &centerlineFile );
    QTextStream debuggerStream(&debuggerFile);

    for (int i = 0; i < size; i++) {

        const ValidPointGroup & vpg = vpgl[i];

        const Pixel & E1 = vpg.leftEdgeI1();
        const Pixel & E2 = vpg.leftEdgeI2();

        int E1ImageX = coordTransformer.imagePointX(E1.u());
        int E1ImageY = coordTransformer.imagePointY(E1.v());

        int E2ImageX = coordTransformer.imagePointX(E2.u());
        int E2ImageY = coordTransformer.imagePointY(E2.v());

        int E1CameraX = coordTransformer.imageX2CameraPointX(E1ImageX);
        int E1CameraY = coordTransformer.imageY2CameraPointY(E1ImageY);
        int E2CameraX = coordTransformer.imageX2CameraPointX(E2ImageX);
        int E2CameraY = coordTransformer.imageY2CameraPointY(E2ImageY);

        double E1ImageXInMeter = E1CameraX*lengthPerPixel;
        double E1ImageYInMeter = E1CameraY*lengthPerPixel;

        double E2ImageXInMeter = E2CameraX*lengthPerPixel;
        double E2ImageYInMeter = E2CameraY*lengthPerPixel;

        Eigen::Vector3d E1coord;
        E1coord(0) = E1ImageXInMeter;
        E1coord(1) = E1ImageYInMeter;
        E1coord(2) = 0.0;

        Eigen::Vector3d E2coord;
        E2coord(0) = E2ImageXInMeter;
        E2coord(1) = E2ImageYInMeter;
        E2coord(2) = 0.0;

        //- determing the normal
        Eigen::Vector3d E1H;
        E1H = H - E1coord;

        Eigen::Vector3d E2H;
        E2H = H - E2coord;

        Eigen::Vector3d vcross(E1H.cross(E2H));

        Eigen::Vector3d p1;
        p1(0) = vcross(0);
        p1(1) = vcross(1);
        p1(2) = vcross(2);

        //- transform to world coordinate system

        p1 = Rinv*(p1-T);

        Eigen::Vector3d normal = p1;

        //- if length of normal == 0
        double length = normal.norm();

        if (length == 0 ) {
            continue;
        }

        double nx = normal(0)/length;
        double ny = normal(1)/length;
        double nz = normal(2)/length;

        const Eigen::Vector3d & centerWorld = vpg.centerWorld();

        double Xc = centerWorld(0);
        double Yc = centerWorld(1);
        double Zc = centerWorld(2);

        Eigen::MatrixXd A = Eigen::MatrixXd::Zero(8,8);
        A.block(0,0,3,4) = P;
        Eigen::VectorXd a = Eigen::VectorXd::Zero(4);
        a << nx, ny, nz, -(nx*Xc+ny*Yc+nz*Zc);

        A(3,0) = a(0);
        A(3,1) = a(1);
        A(3,2) = a(2);
        A(3,3) = a(3);

        A.block(4,4,3,4) = P;

        //- do not use vector to initial a block of matrix!!!
        //A.block(7,4,1,4) = a;
        A(7,4) = a(0);
        A(7,5) = a(1);
        A(7,6) = a(2);
        A(7,7) = a(3);

        Eigen::VectorXd b = Eigen::VectorXd::Zero(8);

        b << E1ImageX, E1ImageY, 1.0, 0.0, E2ImageX, E2ImageY, 1.0, 0.0;

        Eigen::VectorXd x = A.lu().solve(b);

        QFRDebugger dbg;
        dbg.saveFile(A,"Aedge.dat");
        dbg.saveFile(b,"bedge.dat");

        double x1 = x(0)/x(3);
        double y1 = x(1)/x(3);
        double z1 = x(2)/x(3);

        Eigen::Vector3d E1w;
        E1w(0) = x1;
        E1w(1) = y1;
        E1w(2) = z1;

        double x2 = x(4)/x(7);
        double y2 = x(5)/x(7);
        double z2 = x(6)/x(7);

        Eigen::Vector3d E2w;
        E2w(0) = x2;
        E2w(1) = y2;
        E2w(2) = z2;

        edgeStream << "v "<< x1 <<" "<< y1 <<" "<< z1 << endl;
        edgeStream << "v "<< x2 <<" "<< y2 <<" "<< z2 << endl;

        centerlineStream << "v " << Xc <<" " << Yc << " " << Zc << endl;

        const Pixel & centerPixel = vpg.leftCenter();

        debuggerStream << centerPixel.u()
                       << " " << centerPixel.v()
                       << " " << E1.u()
                       << " " << E1.v()
                       << " " << E2.u()
                       << " " << E2.v()
                       << " " << centerPixel.distance(E1)
                       << " " << centerPixel.distance(E2)
                       << " " << (centerWorld-E1w).norm()
                       << " " << (centerWorld-E2w).norm() << endl;
    }

    for(int i = 0; i < size-1; i++) {
        edgeStream << "l " << 2*i+1 <<" "<< 2*(i+1)+1 << endl;
        edgeStream << "l " << 2*i+2 <<" "<< 2*(i+1)+2 << endl;
        centerlineStream << "l " << i+1 <<" "<< i+2 << endl;
    }

    edgeFile.close();
    centerlineFile.close();
    debuggerFile.close();
}

