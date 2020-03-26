#include "SurfaceReconstruction2.h"
#include <QList>
#include <QTextStream>
#include "PixelUVImageXYTransformer.h"
#include <QtMath>

#define EPSILON 1e-8

SurfaceReconstruction2::SurfaceReconstruction2(const ValidPointDetector2 &detector):
    detector_(detector)
{
    setNPoints(20);
    dGamma_ = 2.0*M_PI/nPoints();
}

void SurfaceReconstruction2::setNPoints(int n)
{
    nPoints_ = n;
}

void SurfaceReconstruction2::write()
{
    const CameraPinHole & camL = detector_.camL();
    const CameraPinHole & camR = detector_.camR();

    const QList<ValidPointGroup2> & vpgl = detector_.validPoints();
    const int size = vpgl.size();

    std::cout << "--------------- write ----------------- " << size << std::endl;

    //QString debuggerFilename("debugger.dat");

    QString centerlineFilename;
    centerlineFilename = camL.cameraName() + "_Centerline.obj";
    QFile centerlineFile(centerlineFilename);
    centerlineFile.open(QIODevice::WriteOnly);
    QTextStream centerlineStream( &centerlineFile );

    QString allEdgeFilename;
    allEdgeFilename = camL.cameraName()+"_"+camR.cameraName() + "-Edge.obj";
    QFile allEdgeFile( allEdgeFilename );
    allEdgeFile.open(QIODevice::WriteOnly);
    QTextStream allEdgeStream( &allEdgeFile );

    const int &uMaxL = camL.resolutionU();
    const int &vMaxL = camL.resolutionV();
    const int &uMaxR = camR.resolutionU();
    const int &vMaxR = camR.resolutionV();
    PixelUVImageXYTransformer coordTransformerL(uMaxL,vMaxL);
    PixelUVImageXYTransformer coordTransformerR(uMaxR,vMaxR);

    const double &focalLengthInPixelL = camL.focalLengthInPixel();
    const double &focalLengthInPixelR = camR.focalLengthInPixel();

    const double &lengthPerPixelL = camL.lengthPerPixel();
    const double &lengthPerPixelR = camR.lengthPerPixel();

    const Eigen::Vector3d &H1World = camL.pinHoleCoordWorld();
    const Eigen::Vector3d &H2World = camR.pinHoleCoordWorld();

    int count = 0;
    for (int i = 0; i < size; i++) {

        const Pixel & e11 = vpgl[i].leftEdgeI1();
        const Pixel & e21 = vpgl[i].leftEdgeI2();
        const Pixel & e12 = vpgl[i].rightEdgeI1();
        const Pixel & e22 = vpgl[i].rightEdgeI2();

        //- center will also be optimized in the loop
        // const Eigen::Vector3d &centerWorld = vpgl[i].centerWorld();

        const Eigen::Vector3d &normalWorld = vpgl[i].normalWorld();

        //- camL coordinate system
        const int imageX11 = coordTransformerL.imagePointX(e11.u());
        const int imageY11 = coordTransformerL.imagePointY(e11.v());
        const int imageX21 = coordTransformerL.imagePointX(e21.u());
        const int imageY21 = coordTransformerL.imagePointY(e21.v());

        const int imageX12 = coordTransformerR.imagePointX(e12.u());
        const int imageY12 = coordTransformerR.imagePointY(e12.v());
        const int imageX22 = coordTransformerR.imagePointX(e22.u());
        const int imageY22 = coordTransformerR.imagePointY(e22.v());

        const int cameraX11 = coordTransformerL.imageX2CameraPointX(imageX11);
        const int cameraY11 = coordTransformerL.imageY2CameraPointY(imageY11);
        const int cameraX21 = coordTransformerL.imageX2CameraPointX(imageX21);
        const int cameraY21 = coordTransformerL.imageY2CameraPointY(imageY21);

        const int cameraX12 = coordTransformerR.imageX2CameraPointX(imageX12);
        const int cameraY12 = coordTransformerR.imageY2CameraPointY(imageY12);
        const int cameraX22 = coordTransformerR.imageX2CameraPointX(imageX22);
        const int cameraY22 = coordTransformerR.imageY2CameraPointY(imageY22);

        //- 3d point in camL coordinate system
        Eigen::Vector3d e11L,e21L,e12R,e22R;
        e11L << cameraX11*lengthPerPixelL, cameraY11*lengthPerPixelL, focalLengthInPixelL*lengthPerPixelL;
        e21L << cameraX21*lengthPerPixelL, cameraY21*lengthPerPixelL, focalLengthInPixelL*lengthPerPixelL;

        //- 3d point in camR coordinate system
        e12R << cameraX12*lengthPerPixelR, cameraY12*lengthPerPixelR, focalLengthInPixelR*lengthPerPixelR;
        e22R << cameraX22*lengthPerPixelR, cameraY22*lengthPerPixelR, focalLengthInPixelR*lengthPerPixelR;

        //- direction in camL
        Eigen::Vector3d e11DirectionL = (e11L-camL.pinHoleCoordInMeter());
        Eigen::Vector3d e21DirectionL = (e21L-camL.pinHoleCoordInMeter());
        //- direction in world

        e11DirectionL = camL.R().inverse()*e11DirectionL;
        double length = e11DirectionL.norm();
        e11DirectionL = e11DirectionL/length;

        e21DirectionL = camL.R().inverse()*e21DirectionL;
        length = e21DirectionL.norm();
        e21DirectionL = e21DirectionL/length;

        //- direction in camR
        Eigen::Vector3d e12DirectionR = (e12R-camR.pinHoleCoordInMeter());
        Eigen::Vector3d e22DirectionR = (e22R-camR.pinHoleCoordInMeter());
        //- direction in world
        e12DirectionR = camR.R().inverse()*e12DirectionR;
        length = e12DirectionR.norm();
        e12DirectionR = e12DirectionR/length;

        e22DirectionR = camR.R().inverse()*e22DirectionR;
        length = e22DirectionR.norm();
        e22DirectionR = e22DirectionR/length;

        //- define tau-sigma plane in world coordinate system
        Eigen::Vector3d tau,sigma,kappa;
        Eigen::Vector3d centerWorld;

        //- direction of the line through Center on left camera coordinate system

        const Pixel &pC = vpgl[i].rightCenter();
        double imageCx = coordTransformerR.imagePointX(pC.u());
        double imageCy = coordTransformerR.imagePointY(pC.v());
        double cameraCx = coordTransformerR.imageX2CameraPointX(imageCx);
        double cameraCy = coordTransformerR.imageY2CameraPointY(imageCy);

        Eigen::Vector3d normalC;
        normalC << cameraCx*lengthPerPixelR,cameraCy*lengthPerPixelR,focalLengthInPixelR*lengthPerPixelR;

        //- transform to world coordinate system
        normalC = camR.R().inverse()*normalC;

        double lengthNormalC = normalC.norm();

        normalC(0) = normalC(0)/lengthNormalC;
        normalC(1) = normalC(1)/lengthNormalC;
        normalC(2) = normalC(2)/lengthNormalC;

        double ncx = normalC(0);
        double ncy = normalC(1);
        double ncz = normalC(2);

        //- Newton Method for multivariable

        int nIteration = 0;

        Eigen::Vector4d Res;
        Res << 1.0,1.0,1.0,1.0;
        Eigen::Vector4d solution;
        //- intial guess
        //- a, b, theta, t
        double a = 1.e-3;
        double b = 2.0e-3;
        double theta = 0.0;
        double t = (vpgl[i].centerWorld()(0) - H2World(0))/ncx;
        solution << a,b,theta,t;

        //- start of Newton Method

        Eigen::Matrix4d J;
        Eigen::Vector4d F;
        double A11, B11, C11;
        double A21, B21, C21;
        double A12, B12, C12;
        double A22, B22, C22;
        double A,B,C;
        double dAdt,dBdt,dCdt;

        double Delta11,Delta21,Delta12,Delta22;
        while (Res.norm() > EPSILON && nIteration < 1000)
        {
            nIteration++;

            a = solution(0);
            b = solution(1);
            theta = solution(2);
            t = solution(3);

            //std::cout << "Iteration: " << nIteration <<" " << Res.norm() << std::endl;
            //std::cout << a << " " << b << " " << theta << " " << t << std::endl;

            centerWorld = H2World+t*normalC;

            double xC = centerWorld(0);
            double yC = centerWorld(1);
            double zC = centerWorld(2);

            double dxCdt = ncx;
            double dyCdt = ncy;
            double dzCdt = ncz;

            tau = camR.pinHoleCoordWorld() - centerWorld;
            kappa = normalWorld;
            double kx = kappa(0);
            double ky = kappa(1);
            double kz = kappa(2);

            sigma = kappa.cross(tau);
            double lengthTau = tau.norm();
            double lengthSigma = sigma.norm();
            double lengthKappa = kappa.norm();

            tau = tau/lengthTau;
            sigma = sigma/lengthSigma;
            kappa = kappa/lengthKappa;

            //- project the directions of thest 4 tangent lines to tau-sigma plane
            //- Eigen::Vector3d e11NormalLTauSigma,e21NormalLTauSigma,e12NormalRTauSigma,e22NormalRTauSigma;
            double e11TauComponent = e11DirectionL.dot(tau);
            double e11SigmaComponent = e11DirectionL.dot(sigma);

            double e21TauComponent = e21DirectionL.dot(tau);
            double e21SigmaComponent = e21DirectionL.dot(sigma);

            double e12TauComponent = e12DirectionR.dot(tau);
            double e12SigmaComponent = e12DirectionR.dot(sigma);

            double e22TauComponent = e22DirectionR.dot(tau);
            double e22SigmaComponent = e22DirectionR.dot(sigma);

            Eigen::Vector2d e11TauSigma,e21TauSigma,e12TauSigma,e22TauSigma;

            e11TauSigma << e11TauComponent, e11SigmaComponent;
            e21TauSigma << e21TauComponent, e21SigmaComponent;
            e12TauSigma << e12TauComponent, e12SigmaComponent;
            e22TauSigma << e22TauComponent, e22SigmaComponent;

            Eigen::Vector2d H1TauSigma,H2TauSigma;

            //- H1/H2 projection on tau-sigma plane
            H1TauSigma(0) = (H1World - centerWorld).dot(tau);
            H1TauSigma(1) = (H1World - centerWorld).dot(sigma);
            H2TauSigma(0) = (H2World - centerWorld).dot(tau);
            H2TauSigma(1) = (H2World - centerWorld).dot(sigma);

            //- tangent line expression on tau-sigma plane
            //- (x-xH,y-yH) cross (nx,ny) = 0
            //- ny x - nx y +  nx yH - ny xH = 0
            //- A = ny; B = -nx; C = nx yH - ny xH = -B yH - A xH

            A11 = e11TauSigma(1); B11 = -e11TauSigma(0); C11 = -B11*H1TauSigma(1) - A11*H1TauSigma(0);
            A21 = e21TauSigma(1); B21 = -e21TauSigma(0); C21 = -B21*H1TauSigma(1) - A21*H1TauSigma(0);
            A12 = e12TauSigma(1); B12 = -e12TauSigma(0); C12 = -B12*H2TauSigma(1) - A12*H2TauSigma(0);
            A22 = e22TauSigma(1); B22 = -e22TauSigma(0); C22 = -B22*H2TauSigma(1) - A22*H2TauSigma(0);

            double xH1 = H1World(0); double yH1 = H1World(1); double zH1 = H1World(2);
            double xH2 = H2World(0); double yH2 = H2World(1); double zH2 = H2World(2);

            double groupX = -ky*dzCdt + kz*dyCdt;
            double groupY = kx*dzCdt - kz*dxCdt;
            double groupZ = -kx*dyCdt+ky*dxCdt;

            double groupXX = ky*(zH2-zC) - kz*(yH2-yC);
            double groupYY = -kx*(zH2-zC) + kz*(xH2-xC);
            double groupZZ = kx*(yH2-yC) - ky*(xH2-xC);

            double nx = e11DirectionL(0);
            double ny = e11DirectionL(1);
            double nz = e11DirectionL(2);

            double dA11dt = nx*groupX + ny*groupY + nz*groupZ;
            double dB11dt = nx*dxCdt + ny*dyCdt + nz*dzCdt;

            nx = e21DirectionL(0);
            ny = e21DirectionL(1);
            nz = e21DirectionL(2);

            double dA21dt = nx*groupX + ny*groupY + nz*groupZ;
            double dB21dt = nx*dxCdt + ny*dyCdt + nz*dzCdt;

            nx = e12DirectionR(0);
            ny = e12DirectionR(1);
            nz = e12DirectionR(2);

            double dA12dt = nx*groupX + ny*groupY + nz*groupZ;
            double dB12dt = nx*dxCdt + ny*dyCdt + nz*dzCdt;

            nx = e22DirectionR(0);
            ny = e22DirectionR(1);
            nz = e22DirectionR(2);

            double dA22dt = nx*groupX + ny*groupY + nz*groupZ;
            double dB22dt = nx*dxCdt + ny*dyCdt + nz*dzCdt;

            double dSigma10dt = (xH1-xC)*groupX + (yH1-yC)*groupY + (zH1-zC)*groupZ
                    +(-dxCdt*groupXX)+(-dyCdt*groupYY)+(-dzCdt*groupZZ);
            double dTau10dt = -(xH1-xC)*dxCdt - (yH1-yC)*dyCdt - (zH1-zC)*dzCdt
                    -dxCdt*(xH2-xC)-dyCdt*(yH2-yC)-dzCdt*(zH2-zC);

            double dSigma20dt = (xH2-xC)*groupX + (yH2-yC)*groupY + (zH2-zC)*groupZ
                    +(-dxCdt*groupXX)+(-dyCdt*groupYY)+(-dzCdt*groupZZ);
            double dTau20dt = -(xH2-xC)*dxCdt - (yH2-yC)*dyCdt - (zH2-zC)*dzCdt
                    -dxCdt*(xH2-xC)-dyCdt*(yH2-yC)-dzCdt*(zH2-zC);

            double Tau10 = H1TauSigma(0);
            double Sigma10 = H1TauSigma(1);

            double Tau20 = H2TauSigma(0);
            double Sigma20 = H2TauSigma(1);

            double dC11dt = -(dB11dt*Sigma10 + dSigma10dt*B11 + dA11dt*Tau10 + dTau10dt*A11);
            double dC21dt = -(dB21dt*Sigma10 + dSigma10dt*B21 + dA21dt*Tau10 + dTau10dt*A21);
            double dC12dt = -(dB12dt*Sigma20 + dSigma20dt*B12 + dA12dt*Tau20 + dTau20dt*A12);
            double dC22dt = -(dB22dt*Sigma20 + dSigma20dt*B22 + dA11dt*Tau20 + dTau20dt*A22);

            A = A11;
            B = B11;
            C = C11;
            dBdt = dB11dt;
            dCdt = dC11dt;
            dAdt = dA11dt;

            if(nIteration == 1||1) {
                Delta11 = (A*A - B*B)*(a*a-b*b)*cos(theta)*cos(theta) - 2.0*A*B*(a*a-b*b)*sin(theta)*cos(theta) + A*A*b*b + B*B*a*a - C*C;
            }
            double dDelta11da = (A*A - B*B)*(2.0*a)*cos(theta)*cos(theta) - 2.0*A*B*(2.0*a)*sin(theta)*cos(theta) + B*B*2.0*a;
            double dDelta11db = (A*A - B*B)*(-2.0*b)*cos(theta)*cos(theta) - 2.0*A*B*(-2.0*b)*sin(theta)*cos(theta) + A*A*2.0*b;
            double dDelta11dtheta = (A*A - B*B)*(a*a-b*b)*2.0*(-cos(theta)*sin(theta)) - 2.0*A*B*(a*a-b*b)*cos(2.0*theta);
            double dDelta11dt = (2.0*A*dAdt - 2.0*B*dBdt)*(a*a-b*b)*cos(theta)*cos(theta) - (A*dBdt+B*dAdt)*(a*a-b*b)*sin(2.0*theta)
                    +2.0*A*dAdt*b*b
                    +2.0*B*dBdt*a*a
                    -2.0*C*dCdt;

            A = A21;
            B = B21;
            C = C21;
            dAdt = dA21dt;
            dBdt = dB21dt;
            dCdt = dC21dt;
            if(nIteration == 1||1) {
                Delta21 = (A*A - B*B)*(a*a-b*b)*cos(theta)*cos(theta) - 2.0*A*B*(a*a-b*b)*sin(theta)*cos(theta) + A*A*b*b + B*B*a*a - C*C;
            }
            double dDelta21da = (A*A - B*B)*(2.0*a)*cos(theta)*cos(theta) - 2.0*A*B*(2.0*a)*sin(theta)*cos(theta) +  B*B*2.0*a;
            double dDelta21db = (A*A - B*B)*(-2.0*b)*cos(theta)*cos(theta) - 2.0*A*B*(-2.0*b)*sin(theta)*cos(theta) + A*A*2.0*b;
            double dDelta21dtheta = (A*A - B*B)*(a*a-b*b)*2.0*(-cos(theta)*sin(theta)) - 2.0*A*B*(a*a-b*b)*cos(2.0*theta);
            double dDelta21dt = (2.0*A*dAdt - 2.0*B*dBdt)*(a*a-b*b)*cos(theta)*cos(theta) - (A*dBdt+B*dAdt)*(a*a-b*b)*sin(2.0*theta)
                    +2.0*A*dAdt*b*b
                    +2.0*B*dBdt*a*a
                    -2.0*C*dCdt;

            A = A12;
            B = B12;
            C = C12;
            dAdt = dA12dt;
            dBdt = dB12dt;
            dCdt = dC12dt;
            if(nIteration == 1||1) {
                Delta12 = (A*A - B*B)*(a*a-b*b)*cos(theta)*cos(theta) - 2.0*A*B*(a*a-b*b)*sin(theta)*cos(theta) + A*A*b*b + B*B*a*a - C*C;
            }
            double dDelta12da = (A*A - B*B)*(2.0*a)*cos(theta)*cos(theta) - 2.0*A*B*(2.0*a)*sin(theta)*cos(theta) +  B*B*2.0*a;
            double dDelta12db = (A*A - B*B)*(-2.0*b)*cos(theta)*cos(theta) - 2.0*A*B*(-2.0*b)*sin(theta)*cos(theta) + A*A*2.0*b;
            double dDelta12dtheta = (A*A - B*B)*(a*a-b*b)*2.0*(-cos(theta)*sin(theta)) - 2.0*A*B*(a*a-b*b)*cos(2.0*theta);
            double dDelta12dt = (2.0*A*dAdt - 2.0*B*dBdt)*(a*a-b*b)*cos(theta)*cos(theta) - (A*dBdt+B*dAdt)*(a*a-b*b)*sin(2.0*theta)
                    +2.0*A*dAdt*b*b
                    +2.0*B*dBdt*a*a
                    -2.0*C*dCdt;

            A = A22;
            B = B22;
            C = C22;
            dAdt = dA22dt;
            dBdt = dB22dt;
            dCdt = dC22dt;
            if(nIteration == 1||1) {
                Delta22 = (A*A - B*B)*(a*a-b*b)*cos(theta)*cos(theta) - 2.0*A*B*(a*a-b*b)*sin(theta)*cos(theta) + A*A*b*b + B*B*a*a - C*C;
            }
            double dDelta22da = (A*A - B*B)*(2.0*a)*cos(theta)*cos(theta) - 2.0*A*B*(2.0*a)*sin(theta)*cos(theta) +  B*B*2.0*a;
            double dDelta22db = (A*A - B*B)*(-2.0*b)*cos(theta)*cos(theta) - 2.0*A*B*(-2.0*b)*sin(theta)*cos(theta) + A*A*2.0*b;
            double dDelta22dtheta = (A*A - B*B)*(a*a-b*b)*2.0*(-cos(theta)*sin(theta)) - 2.0*A*B*(a*a-b*b)*cos(2.0*theta);
            double dDelta22dt = (2.0*A*dAdt - 2.0*B*dBdt)*(a*a-b*b)*cos(theta)*cos(theta) - (A*dBdt+B*dAdt)*(a*a-b*b)*sin(2.0*theta)
                    +2.0*A*dAdt*b*b
                    +2.0*B*dBdt*a*a
                    -2.0*C*dCdt;

            //- compose Jacob matrix

            if(nIteration == 1||1) {
                J.row(0) << dDelta11da,dDelta11db,dDelta11dtheta,dDelta11dt;
                J.row(1) << dDelta21da,dDelta21db,dDelta21dtheta,dDelta21dt;
                J.row(2) << dDelta12da,dDelta12db,dDelta12dtheta,dDelta12dt;
                J.row(3) << dDelta22da,dDelta22db,dDelta22dtheta,dDelta22dt;
            }

            F(0) = Delta11;
            F(1) = Delta21;
            F(2) = Delta12;
            F(3) = Delta22;

            Res = J.inverse()*F;

//            std::cout << "J: " << J << std::endl;
//            std::cout << "J-1: " << J.inverse() << std::endl;
            //std::cout << "Residual: " << Res.norm() << std::endl;
//            std::cout << "func: " << F << std::endl;

            solution = solution - 0.1*Res;

            //std::cout << a << " " << b << " " << theta << " " << t << std::endl;

        }

        if (Res.norm() > EPSILON || a < 0 || b < 0) {
            continue;
        }

        count++;
        std::cout << i <<", ";
        //- start write edge
        if (i%50 == 0 || 1)
        {
            //- update solutions
            a = solution(0);
            b = solution(1);
            theta = solution(2);
            t = solution(3);

            centerWorld = H2World+t*normalC;

            tau = camR.pinHoleCoordWorld() - centerWorld;
            sigma = normalWorld.cross(tau);

            double lengthTau = tau.norm();
            double lengthSigma = sigma.norm();

            tau = tau/lengthTau;
            sigma = sigma/lengthSigma;

            CrossSectionProfile ellipse;
            QList<Eigen::Vector3d> ellipse3d;

            Eigen::Vector3d nxWorld,nyWorld,nzWorld;
            nxWorld << 1.0, 0.0, 0.0;
            nyWorld << 0.0, 1.0, 0.0;
            nzWorld << 0.0, 0.0, 1.0;

            std::cout << "DELTA: " << F << std::endl;
            std::cout << "a,b,theta,t: "<< a << " " << b << " " << theta << " " << t << std::endl;
            std::cout << "A,B,C: "<<A11 << " " << B11 << " " << C11 << std::endl;
            std::cout << "A,B,C: "<<A21 << " " << B21 << " " << C21 << std::endl;
            std::cout << "A,B,C: "<<A12 << " " << B12 << " " << C12 << std::endl;
            std::cout << "A,B,C: "<<A22 << " " << B22 << " " << C22 << std::endl;

            QString edgeFilename;
            edgeFilename = camL.cameraName()+"_"+camR.cameraName() +"-"+QString::number(i)+ "-Edge.obj";
            QFile edgeFile( edgeFilename );
            edgeFile.open(QIODevice::WriteOnly);
            QTextStream edgeStream( &edgeFile );

            for (int gammaI = 0; gammaI < nPoints(); gammaI++){

                double gamma = dGamma()*gammaI;
                double TAU = a*cos(theta)*cos(gamma)+b*sin(theta)*sin(gamma);
                double SIGMA = b*cos(theta)*sin(gamma) - a*sin(theta)*cos(gamma);
                ellipse.x().append(TAU);
                ellipse.y().append(SIGMA);

                Eigen::Vector3d point3dTauSigma,point3dWorld;
                point3dTauSigma(0) = TAU;
                point3dTauSigma(1) = SIGMA;
                point3dTauSigma(2) = 0.0; //centerWorld(2);

                // Px = P_tau (tau x) + P_sigma (sigma x) + P_kappa (kappa x) + Cx
                point3dWorld(0) = point3dTauSigma(0)*(tau.dot(nxWorld))
                        + point3dTauSigma(1)*(sigma.dot(nxWorld))
                        + point3dTauSigma(2)*(kappa.dot(nxWorld))
                        + centerWorld(0);

                point3dWorld(1) = point3dTauSigma(0)*(tau.dot(nyWorld))
                        + point3dTauSigma(1)*(sigma.dot(nyWorld))
                        + point3dTauSigma(2)*(kappa.dot(nyWorld))
                        + centerWorld(1);

                point3dWorld(2) = point3dTauSigma(0)*(tau.dot(nzWorld))
                        + point3dTauSigma(1)*(sigma.dot(nzWorld))
                        + point3dTauSigma(2)*(kappa.dot(nzWorld))
                        + centerWorld(2);
                ellipse3d.append(point3dWorld);

                edgeStream << "v " << point3dWorld(0) << " " << point3dWorld(1) << " " << point3dWorld(2) << endl;
                allEdgeStream << "v " << point3dWorld(0) << " " << point3dWorld(1) << " " << point3dWorld(2) << endl;
                //edgeStream << "v " << TAU << " " << SIGMA << " " << 0.0 << endl;

            }

            for(int dd = 0; dd < nPoints(); dd++) {
                if (dd+1 < 20){
                    edgeStream << "l " << dd+1 << " " << dd+2 << endl;
                } else if (dd+1 == 20) {
                    edgeStream << "l " << dd+1 << " " << 1 << endl;
                }
            }

            crossSections_.append(ellipse);
            crossSectionsWorld_.append(ellipse3d);

            centerlineStream << "v " << centerWorld(0) << " " << centerWorld(1) << " " << centerWorld(2) << endl;

            QFile e11File( "e11-"+QString::number(i)+".obj" );
            e11File.open(QIODevice::WriteOnly);
            QTextStream e11Stream( &e11File );

            QFile e21File( "e21-"+QString::number(i)+".obj" );
            e21File.open(QIODevice::WriteOnly);
            QTextStream e21Stream( &e21File );

            QFile e12File( "e12-"+QString::number(i)+".obj" );
            e12File.open(QIODevice::WriteOnly);
            QTextStream e12Stream( &e12File );

            QFile e22File( "e22-"+QString::number(i)+".obj" );
            e22File.open(QIODevice::WriteOnly);
            QTextStream e22Stream( &e22File );

            QFile centerFile( "center-"+QString::number(i)+".obj" );
            centerFile.open(QIODevice::WriteOnly);
            QTextStream centerStream( &centerFile );

            double delta = -1.0/1000;
            for(int dd = 0; dd < 300; dd++){
                Eigen::Vector3d E11W,E21W,E12W,E22W,CenterW;
                E11W = H1World + e11DirectionL*delta*dd;
                E21W = H1World + e21DirectionL*delta*dd;
                E12W = H2World + e12DirectionR*delta*dd;
                E22W = H2World + e22DirectionR*delta*dd;
                CenterW = H2World + normalC*delta*dd;
                e11Stream << "v " << E11W(0) << " " << E11W(1) << " " << E11W(2) << endl;
                e21Stream << "v " << E21W(0) << " " << E21W(1) << " " << E21W(2) << endl;
                e12Stream << "v " << E12W(0) << " " << E12W(1) << " " << E12W(2) << endl;
                e22Stream << "v " << E22W(0) << " " << E22W(1) << " " << E22W(2) << endl;
                centerStream <<"v " << CenterW(0) << " " << CenterW(1) << " " << CenterW(2) << endl;
            }

            for(int dd = 0; dd < 300; dd++) {
                if (dd+1 < 300){
                    e11Stream << "l " << dd+1 << " " << dd+2 << endl;
                    e21Stream << "l " << dd+1 << " " << dd+2 << endl;
                    e12Stream << "l " << dd+1 << " " << dd+2 << endl;
                    e22Stream << "l " << dd+1 << " " << dd+2 << endl;
                    centerStream << "l " << dd+1 << " " << dd+2 << endl;
                }
            }

            e11File.close();
            e21File.close();
            e12File.close();
            e22File.close();
            centerFile.close();
            edgeFile.close();
        }

        //- end write edge
    }

    std::cout << "\n------ total passed : " << count << std::endl;

    allEdgeFile.close();
    centerlineFile.close();

}
