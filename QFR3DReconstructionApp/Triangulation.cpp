#include "Triangulation.h"
#include <Eigen/Core>
#include <Eigen/Dense>
#include <Eigen/SVD>
#include "QFRDebugger.h"

using namespace Eigen;

//Triangulation::Triangulation()
//{

//}

Triangulation::Triangulation(const Eigen::Matrix3Xd &p1, const Eigen::Matrix3Xd &p2)
{
    P1_ = p1;
    P2_ = p2;
}

Eigen::Vector3d Triangulation::construct3DPoint(const Eigen::Vector3d &p1, const Eigen::Vector3d & p2)
{
    Eigen::Vector3d p;

    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(6,6);
    A.block(0,0,3,4) = P1_;
    A.block(3,0,3,4) = P2_;
    A.block(0,4,3,1) = -p1;
    A.block(3,5,3,1) = -p2;

//    QFRDebugger dbg;
//    dbg.saveFile(A,"A.dat");

    Eigen::JacobiSVD<Eigen::MatrixXd> svd_holder(A,Eigen::ComputeFullU | Eigen::ComputeFullV);

    Eigen::MatrixXd U = svd_holder.matrixU();
    Eigen::MatrixXd V = svd_holder.matrixV();
    Eigen::MatrixXd D = svd_holder.singularValues();

//    dbg.saveFile(U,"U.dat");
//    dbg.saveFile(V,"V.dat");
//    dbg.saveFile(D,"D.dat");

    Eigen::Vector4d x;
    //x = V.block(5,0,1,4);
    x(0) = V(0,5);
    x(1) = V(1,5);
    x(2) = V(2,5);
    x(3) = V(3,5);

    p(0) = x(0)/x(3);
    p(1) = x(1)/x(3);
    p(2) = x(2)/x(3);

//    qDebug() << "solution: " << x(0) << " " << x(1) << " " << x(2) << " " << x(3) << endl;

    return p;
}
