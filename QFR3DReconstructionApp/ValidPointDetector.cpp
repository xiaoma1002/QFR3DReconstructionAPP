#include "ValidPointDetector.h"
#include "ExtractSingleCurveFromImage.h"
#include "ExtractTwoEdgesFromContour.h"
#include "Triangulation.h"
#include "QFRDebugger.h"
#include <QDebug>
#include "PixelToLineDistanceMeasurer.h"
#include "EstimateNormalDirectionPixelCurve.h"

//ValidPointDetector::ValidPointDetector():
//    camL_(nullptr),
//    camR_(nullptr),
//    F_(nullptr)
//{

//}

ValidPointDetector::ValidPointDetector(\
        const CameraPinHole &camL, \
        const CameraPinHole &camR):
    camL_(camL),
    camR_(camR)
{
}

void ValidPointDetector::findValidPoints(const Eigen::Matrix3d &F12, const Eigen::Matrix3d &F21)
{
    //- find valid points and store in validPoints_

    //- step1: extract left segments from left image

    ExtractSingleCurveFromImage escfiL(camL_.skeletonImage());
    ExtractSingleCurveFromImage escfiR(camR_.skeletonImage());

    QFRDebugger dbg;

    //dbg.display("skeL",camL_.skeletonImage());
    //dbg.display("skeR",camR_.skeletonImage());

    const QList<Pixel> endL = escfiL.findEnds();
    const QList<Pixel> endR = escfiR.findEnds();

    //- determine the head/tail of left centerline
    //- end[0] as the head
    //- end[1] as the tail
    //- find the head/tail of the right centerline
    //- methodology:
    //- take the head of left curve and find the pixel with shortest
    //- distance to the epiploar line on the right image as the head
    const int &uMaxL = camL_.resolutionU();
    const int &vMaxL = camL_.resolutionV();
    const int &uMaxR = camR_.resolutionU();
    const int &vMaxR = camR_.resolutionV();
    PixelUVImageXYTransformer coordTransformerL(uMaxL,vMaxL);
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

    const PixelCurve centerlineL = escfiL.findCurveByGivingHead(endL[0]);
    const PixelCurve centerlineR = escfiR.findCurveByGivingHead(headR);

    dbg.display("left centerline",camL_.skeletonImage(),centerlineL.curve());
    dbg.display("right centerline",camR_.skeletonImage(),centerlineR.curve());

    //- step2: extract two edges from left image

    //- step3: perform valid point detection on left image

    findValidPoints(camL_,camR_,centerlineL,centerlineR,coordTransformerL,coordTransformerR,F12,validPointsL_);
    findValidPoints(camR_,camL_,centerlineR,centerlineL,coordTransformerR,coordTransformerL,F21,validPointsR_);

}

void ValidPointDetector::findValidPoints(const CameraPinHole & cameraL,
                                          const CameraPinHole & cameraR,
                                          const PixelCurve & centerlineL,
                                          const PixelCurve & centerlineR,
                                          PixelUVImageXYTransformer &coordTransformerL,
                                          PixelUVImageXYTransformer &coordTransformerR,
                                          const Eigen::Matrix3d &F,
                                          QList<ValidPointGroup>& validPoints)
{
    ExtractTwoEdgesFromContour etefc(centerlineL, cameraL.contourImage());
    PixelCurve edge1,edge2;
    etefc.extract(edge1,edge2);

    int size = centerlineL.curve().size();
    QFRDebugger dbg;
    dbg.display(cameraL.cameraName()+"-edge1",cameraL.skeletonImage(),edge1.curve());
    dbg.display(cameraL.cameraName()+"-edge2",cameraL.skeletonImage(),edge2.curve());

    FindEpiloarIntersectionForPair feifp(cameraR,F,centerlineR);

    FindUniqueIntersectionLineAndCurve fuilc1(edge1);
    FindUniqueIntersectionLineAndCurve fuilc2(edge2);

    Triangulation triangulation(cameraL.P(),cameraR.P());

    EstimateNormalDirectionPixelCurve estDirection(centerlineL);

    QList<Pixel> centerPixel1;
    QList<Pixel> centerPixel2;

    for (int i = 0; i < size; i++) {
        const Pixel &pixelIth = centerlineL.curve()[i];
        int u = pixelIth.u();
        int v = pixelIth.v();
        int imageX = coordTransformerL.imagePointX(u);
        int imageY = coordTransformerL.imagePointY(v);
        Eigen::Vector3d x;
        x(0) = imageX;
        x(1) = imageY;
        x(2) = 1.0;
        Pixel interI;
        bool foundEpipolarPair = false;
        foundEpipolarPair = feifp.find(x,interI);
        //dbg.display("searching Area", cameraL.skeletonImage(),feifp.searchingArea());

        if (foundEpipolarPair) {

            centerPixel1.append(pixelIth);
            centerPixel2.append(interI);

            bool foundEdgeIntersection1 = false;
            bool foundEdgeIntersection2 = false;

            Eigen::Vector3d cutLine;
            const Eigen::Vector2d &direction = estDirection.orthNormal()[i];
            cutLine(0) = direction(1);
            cutLine(1) = -direction(0);
            cutLine(2) = direction(0)*v-direction(1)*u;

            Eigen::Vector2d currentPixel;
            currentPixel(0) = pixelIth.u();
            currentPixel(1) = pixelIth.v();

            foundEdgeIntersection1 = fuilc1.findIntersection(cutLine,currentPixel);
            foundEdgeIntersection2 = fuilc2.findIntersection(cutLine,currentPixel);

            if (foundEdgeIntersection1 && foundEdgeIntersection2) {

                Eigen::Vector3d p1;
                Eigen::Vector3d p2;
                p1(0) = coordTransformerL.imagePointX(pixelIth.u());
                p1(1) = coordTransformerL.imagePointY(pixelIth.v());
                p1(2) = 1.0;

                p2(0) = coordTransformerR.imagePointX(interI.u());
                p2(1) = coordTransformerR.imagePointY(interI.v());
                p2(2) = 1.0;

                Eigen::Vector3d p = triangulation.construct3DPoint(p1,p2);

                ValidPointGroup vpg;

                const Pixel & e1 = fuilc1.inter();
                const Pixel & e2 = fuilc2.inter();

                vpg.setLeftEdgeI1(e1);
                vpg.setLeftEdgeI2(e2);

                vpg.setLeftCenter(pixelIth);
                vpg.setRightCenter(interI);

                vpg.setCenterWorld(p);

                validPoints.append(vpg);
            }

        } else {
            continue;
        }
    }
    dbg.displayOverlay("epipolarline",cameraR.skeletonImage(),feifp.epipolarLine());
    dbg.display("used center pixel-left",cameraL.skeletonImage(),centerPixel1);
    dbg.display("used center pixel-right",cameraR.skeletonImage(),centerPixel2);
}
