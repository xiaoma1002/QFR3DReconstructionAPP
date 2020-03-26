#include "ValidPointDetector2.h"
#include "ExtractSingleCurveFromImage.h"
#include "PixelUVImageXYTransformer.h"
#include "EstimateNormalDirectionPixelCurve.h"
#include "FindEpiloarIntersectionForPair.h"
#include "FindUniqueIntersectionLineAndCurve.h"
#include "PixelToLineDistanceMeasurer.h"
#include "ExtractTwoEdgesFromContour.h"
#include "Triangulation.h"
#include "FindEdgePointForCamera.h"
#include "QFRDebugger.h"

ValidPointDetector2::ValidPointDetector2(const CameraPinHole &camL, const CameraPinHole &camR):
    camL_(camL),
    camR_(camR)
{
}

void ValidPointDetector2::findValidPoints(const Eigen::Matrix3d &F12)
{
    ExtractSingleCurveFromImage escfiL(camL().skeletonImage());
    ExtractSingleCurveFromImage escfiR(camR().skeletonImage());
    const QList<Pixel> endL = escfiL.findEnds();
    const QList<Pixel> endR = escfiR.findEnds();

    //- assume the head is endL[0]
    const PixelCurve centerlineL = escfiL.findCurveByGivingHead(endL[0]);

    QFRDebugger dbg;
//    dbg.display("centerline left",camL().skeletonImage(),centerlineL.curve());

//    EstimateNormalDirectionPixelCurve estimateNormalL(centerlineL);

//    std::cout << "normal direction" << std::endl;
//    std::cout << estimateNormalL << std::endl;

    const int &uMaxL = camL().resolutionU();
    const int &vMaxL = camL().resolutionV();
    PixelUVImageXYTransformer coordTransformerL(uMaxL,vMaxL);
    const int &uMaxR = camR().resolutionU();
    const int &vMaxR = camR().resolutionV();
    PixelUVImageXYTransformer coordTransformerR(uMaxR,vMaxR);

    int uL = endL[0].u();
    int vL = endL[0].v();
    int imageXL = coordTransformerL.imagePointX(uL);
    int imageYL = coordTransformerL.imagePointY(vL);
    Eigen::Vector3d xL;
    xL(0) = imageXL;
    xL(1) = imageYL;
    xL(2) = 1.0;

    //- prepare the epipolar line in uv pixel coordinate system
    Eigen::Vector3d lineR;
    lineR = F12*xL;
    double b = lineR(1);
    double c = lineR(2);
    lineR(1) = -b;
    lineR(2) = c + b*vMaxL;

    PixelToLineDistanceMeasurer pixelToLineDistanceMeasurer(lineR,endR);
    const Pixel headR = pixelToLineDistanceMeasurer.nearestPixel();
    const PixelCurve centerlineR = escfiR.findCurveByGivingHead(headR);

//    dbg.display("centerline right",camR().skeletonImage(),centerlineR.curve());

    ExtractTwoEdgesFromContour etefc1(centerlineL, camL().contourImage());
    ExtractTwoEdgesFromContour etefc2(centerlineR, camR().contourImage());
    PixelCurve edgeL1,edgeL2,edgeR1,edgeR2;

    etefc1.extract(edgeL1,edgeL2);
    etefc2.extract(edgeR1,edgeR2);

//    dbg.display("edge1 left",camL().skeletonImage(),edgeL1.curve());
//    dbg.display("edge2 left",camL().skeletonImage(),edgeL2.curve());
//    dbg.display("edge1 right",camR().skeletonImage(),edgeR1.curve());
//    dbg.display("edge2 right",camR().skeletonImage(),edgeR2.curve());

    int size = centerlineL.curve().size();

    FindEpiloarIntersectionForPair feifp(camR(),F12,centerlineR);

    FindUniqueIntersectionLineAndCurve fuilcL1(edgeL1);
    FindUniqueIntersectionLineAndCurve fuilcL2(edgeL2);
    FindUniqueIntersectionLineAndCurve fuilcR1(edgeR1);
    FindUniqueIntersectionLineAndCurve fuilcR2(edgeR2);

    Triangulation triangulation(camL().P(),camR().P());

//    FindEdgePointForCamera fepfcL(camL());
//    FindEdgePointForCamera fepfcR(camR());

    //- loop over all pixels over centerline
    //- find the epipolar pair-> find world coordinate of center
    //- find e11,e21 -> find E11,E21 in world, then construct a plane
    //- which cuts the right image plane for determing the e12,e22

    for (int i = 0; i < size; i++) {
        const Pixel &pixelIth = centerlineL.curve()[i];
        //- P0-uv
        int uL = pixelIth.u();
        int vL = pixelIth.v();
        int imageXL = coordTransformerL.imagePointX(uL);
        int imageYL = coordTransformerL.imagePointY(vL);
        //- P0-xy
        Eigen::Vector3d xL;
        xL(0) = imageXL;
        xL(1) = imageYL;
        xL(2) = 1.0;
        Pixel interI;
        bool foundEpipolarPair = false;
        foundEpipolarPair = feifp.find(xL,interI);

        if (foundEpipolarPair) {

            //- construct center in world
            Eigen::Vector3d pL;
            Eigen::Vector3d pR;
            pL(0) = coordTransformerL.imagePointX(pixelIth.u());
            pL(1) = coordTransformerL.imagePointY(pixelIth.v());
            pL(2) = 1.0;

            pR(0) = coordTransformerR.imagePointX(interI.u());
            pR(1) = coordTransformerR.imagePointY(interI.v());
            pR(2) = 1.0;

            const Eigen::Vector3d p = triangulation.construct3DPoint(pL,pR);

            //- define the plane of H1-C-H2 in world coordinate system

            const Eigen::Vector3d & HL = camL().pinHoleCoordWorld();
            const Eigen::Vector3d & HR = camR().pinHoleCoordWorld();

            //- define normal
            Eigen::Vector3d normal,normalL,normalR;
            normal = (HL-p).cross(HR-p);

            double length = normal.norm();
            normal = normal/length;

            normalL = camL().R()*normal;
            normalR = camR().R()*normal;

            double lengthL = normalL.norm();
            normalL = normalL/lengthL;

            double lengthR = normalR.norm();
            normalR = normalR/lengthR;

            Eigen::Vector3d pointThroughL = camL().pinHoleCoordInMeter();
            Eigen::Vector3d pointThroughR = camR().pinHoleCoordInMeter();

            double focalLengthInMeterL = camL().focalLengthInMeter();
            double focalLengthInMeterR = camR().focalLengthInMeter();

            const double & lengthPerPixelL = camL().lengthPerPixel();
            const double & lengthPerPixelR = camR().lengthPerPixel();

            //- Ax+By+Cz+D = 0 && Z = f; (in image coordinate system, meter units)
            //- Ax+By+Cf+D = 0;
            double A = normalL(0);
            double B = normalL(1);
            double C = normalL(2);
            double D = -(normalL(0)*pointThroughL(0) +
                         normalL(1)*pointThroughL(1) +
                         normalL(2)*pointThroughL(2));

            //- still in X'-O-Y' coordinate system
            //- where O has (0.5uMax,0.5vMax)
            double a1 = A;
            double b1 = B;
            double c1 = C*focalLengthInMeterL+D;

            //- convert to X-O-Y coordinate system
            //- where O has (0,0)
            double a2 = a1;
            double b2 = b1;
            double c2 = c1 - a1*camL().cx()*lengthPerPixelL- b1*camL().cy()*lengthPerPixelL;

            //- conver to pixel coordinate system
            double a = a2;
            double b = -b2;
            double c = vMaxL*b2*lengthPerPixelL + c2;

            a = a*lengthPerPixelL;
            b = b*lengthPerPixelL;

            bool foundEdgeIntersectionL1 = false;
            bool foundEdgeIntersectionL2 = false;

            Eigen::Vector3d lineL;
            lineL << a, b, c;

            //std::cout <<" through test: " << a*pixelIth.u() + b*pixelIth.v() +c << std::endl;
            Eigen::Vector2d closestL;
            closestL(0) = pixelIth.u();
            closestL(1) = pixelIth.v();
            foundEdgeIntersectionL1 = fuilcL1.findIntersection(lineL,closestL);
            foundEdgeIntersectionL2 = fuilcL2.findIntersection(lineL,closestL);

            if (foundEdgeIntersectionL1 && foundEdgeIntersectionL2) {
                //- Ax+By+Cz+D = 0 && Z = 0; (in image coordinate system)
                //- Ax+By+Cf+D = 0;
                double A = normalR(0);
                double B = normalR(1);
                double C = normalR(2);
                double D = -(normalR(0)*pointThroughR(0) +
                             normalR(1)*pointThroughR(1) +
                             normalR(2)*pointThroughR(2));

                //- still in X'-O-Y' coordinate system
                //- where O has (0.5uMax,0.5vMax)
                double a1 = A;
                double b1 = B;
                double c1 = C*focalLengthInMeterR+D;

                //- convert to X-O-Y coordinate system
                //- where O has (0,0)
                double a2 = a1;
                double b2 = b1;
                double c2 = c1 - a1*camR().cx()*lengthPerPixelR- b1*camR().cy()*lengthPerPixelR;

                //- conver to pixel coordinate system
                double a = a2;
                double b = -b2;
                double c = vMaxR*b2*lengthPerPixelR + c2;

                a = a*lengthPerPixelR;
                b = b*lengthPerPixelR;

                bool foundEdgeIntersectionR1 = false;
                bool foundEdgeIntersectionR2 = false;

                Eigen::Vector3d lineR;
                lineR << a, b, c;
                Eigen::Vector2d closestR;
                closestR(0) = interI.u();
                closestR(1) = interI.v();
                foundEdgeIntersectionR1 = fuilcR1.findIntersection(lineR,closestR);
                foundEdgeIntersectionR2 = fuilcR2.findIntersection(lineR,closestR);

                if (foundEdgeIntersectionR1 && foundEdgeIntersectionR2) {
                    //- construct e11 and e21
                    const Pixel &e1L = fuilcL1.inter();
                    const Pixel &e2L = fuilcL2.inter();
                    //Eigen::Vector3d E1wL, E2wL;

                    //fepfcL.construct2(e1L,e2L,p,normal,E1wL,E2wL);

                    //std::cout <<" same normal? " << normal << normal2 << std::endl;

                    //- construct e12 and e22
                    const Pixel &e1R = fuilcR1.inter();
                    const Pixel &e2R = fuilcR2.inter();
                    //Eigen::Vector3d E1wR, E2wR;

                    //fepfcR.construct2(e1R,e2R,p,normal,E1wR,E2wR);

                    //std::cout <<" + "<< normal << std::endl;

                    ValidPointGroup2 vpg;
                    vpg.setCenterWorld(p);
                    vpg.setLeftCenter(pixelIth);
                    vpg.setRightCenter(interI);
                    vpg.setLeftEdgeI1(e1L);
                    vpg.setLeftEdgeI2(e2L);
                    vpg.setRightEdgeI1(e1R);
                    vpg.setRightEdgeI2(e2R);
                    vpg.setNormalWorld(normal);
                    /*
                    vpg.setLeftEdgeI1World(E1wL);
                    vpg.setLeftEdgeI2World(E2wL);
                    vpg.setRightEdgeI1World(E1wR);
                    vpg.setRightEdgeI2World(E2wR);
                    */
                    validPoints_.append(vpg);
                }
            }
        }

    }

    dbg.displayOverlay("epipolar line", camR().skeletonImage(),feifp.epipolarLine());

}
