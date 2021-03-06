#include "FindEdgePointForCamera.h"
#include "PixelUVImageXYTransformer.h"
#include <QMessageBox>
#include <QApplication>
#include "QFRDebugger.h"


FindEdgePointForCamera::FindEdgePointForCamera(const CameraPinHole & camera)
    :camera_(camera)
{

}

Eigen::Vector3d FindEdgePointForCamera::construct(const Pixel &e1,
                                  const Pixel &e2,
                                  const Eigen::Vector3d& xWorld,
                                  Eigen::Vector3d& E1w,
                                  Eigen::Vector3d& E2w)
{
    Eigen::Vector3d H = Eigen::Vector3d::Zero();
    //H(2) = -camera().focalLengthInMeter();

    const int & uMax = camera().resolutionU();
    const int & vMax = camera().resolutionV();

    const double & lengthPerPixel = camera().lengthPerPixel();

    //- inverse of R1
    Eigen::Matrix3d Rinv = camera().R().inverse();

    //- camera translation vector in relative to world's origin
    const Eigen::Vector3d &T = camera().t();

    const Eigen::Matrix3Xd &P = camera().P();

    PixelUVImageXYTransformer coordTransformer(uMax,vMax);

    const Pixel & E1 = e1;

    int E1ImageX = coordTransformer.imagePointX(E1.u());
    int E1ImageY = coordTransformer.imagePointY(E1.v());

    double E1ImageXInMeter = coordTransformer.imageX2CameraPointX(E1ImageX)*lengthPerPixel;
    double E1ImageYInMeter = coordTransformer.imageY2CameraPointY(E1ImageY)*lengthPerPixel;
    Eigen::Vector3d E1coord;
    E1coord(0) = E1ImageXInMeter;
    E1coord(1) = E1ImageYInMeter;
    E1coord(2) = camera().focalLengthInMeter();

    const Pixel & E2 = e2;

    int E2ImageX = coordTransformer.imagePointX(E2.u());
    int E2ImageY = coordTransformer.imagePointY(E2.v());

    double E2ImageXInMeter = coordTransformer.imageX2CameraPointX(E2ImageX)*lengthPerPixel;
    double E2ImageYInMeter = coordTransformer.imageX2CameraPointX(E2ImageY)*lengthPerPixel;
    Eigen::Vector3d E2coord;
    E2coord(0) = E2ImageXInMeter;
    E2coord(1) = E2ImageYInMeter;
    E2coord(2) = camera().focalLengthInMeter();

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

    Eigen::Vector3d pOrigin;
    pOrigin = H;
    pOrigin = Rinv*(pOrigin-T);

    Eigen::Vector3d normal = pOrigin - p1;

    //- if length of normal == 0
    double length = normal.norm();

    if (length == 0) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," zero length normal (FindEdgePointForCamera)!");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }

    double nx = normal(0)/length;
    double ny = normal(1)/length;
    double nz = normal(2)/length;

    const Eigen::Vector3d & centerWorld = xWorld;

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

    Eigen::JacobiSVD<Eigen::MatrixXd> svd_holder(A,Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::VectorXd x = svd_holder.solve(b);

    double x1 = x(0)/x(3);
    double y1 = x(1)/x(3);
    double z1 = x(2)/x(3);

    E1w(0) = x1;
    E1w(1) = y1;
    E1w(2) = z1;

    double x2 = x(4)/x(7);
    double y2 = x(5)/x(7);
    double z2 = x(6)/x(7);

    E2w(0) = x2;
    E2w(1) = y2;
    E2w(2) = z2;

    Eigen::Vector3d n;
    n << nx,ny,nz;

    return n;
}

void FindEdgePointForCamera::construct2(
        const Pixel &e1,
        const Pixel &e2,
        const Eigen::Vector3d &xWorld,
        const Eigen::Vector3d &normal,
        Eigen::Vector3d &E1w,
        Eigen::Vector3d &E2w)
{
    //- if length of normal == 0
    double length = normal.norm();

    if (length == 0) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," zero length normal (FindEdgePointForCamera)!");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }

    double nx = normal(0)/length;
    double ny = normal(1)/length;
    double nz = normal(2)/length;

    const int & uMax = camera().resolutionU();
    const int & vMax = camera().resolutionV();

    const Eigen::Matrix3Xd &P = camera().P();

    PixelUVImageXYTransformer coordTransformer(uMax,vMax);

    const Pixel & E1 = e1;

    int E1ImageX = coordTransformer.imagePointX(E1.u());
    int E1ImageY = coordTransformer.imagePointY(E1.v());

    const Pixel & E2 = e2;

    int E2ImageX = coordTransformer.imagePointX(E2.u());
    int E2ImageY = coordTransformer.imagePointY(E2.v());


    const Eigen::Vector3d & centerWorld = xWorld;

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

//    QFRDebugger dbg;
//    dbg.saveFile(A,"A.dat");
//    dbg.saveFile(b,"b.dat");

    //Eigen::VectorXd x = A.lu().solve(b);
    //Eigen::VectorXd x = A.colPivHouseholderQr().solve(b);

    Eigen::JacobiSVD<Eigen::MatrixXd> svd_holder(A,Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::VectorXd x = svd_holder.solve(b);

//    Eigen::MatrixXd U = svd_holder.matrixU();
//    Eigen::MatrixXd V = svd_holder.matrixV();
//    Eigen::MatrixXd D = svd_holder.singularValues();

//    std::cout << "E1&E2 " << E1 << E2 << std::endl;
//    std::cout <<"A "<< A << std::endl;
//    std::cout <<"b "<< b << std::endl;
//    std::cout <<"x" << x << std::endl;

    double x1 = x(0)/x(3);
    double y1 = x(1)/x(3);
    double z1 = x(2)/x(3);

    E1w(0) = x1;
    E1w(1) = y1;
    E1w(2) = z1;

    double x2 = x(4)/x(7);
    double y2 = x(5)/x(7);
    double z2 = x(6)/x(7);

    E2w(0) = x2;
    E2w(1) = y2;
    E2w(2) = z2;
}
