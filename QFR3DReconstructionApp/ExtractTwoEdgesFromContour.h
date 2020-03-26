#ifndef EXTRACTTWOEDGESFROMCONTOUR_H
#define EXTRACTTWOEDGESFROMCONTOUR_H

#include <QList>
#include "Pixel.h"
#include "opencv2/opencv.hpp"
#include "PixelCurve.h"

class ExtractTwoEdgesFromContour
{

public:

//    ExtractTwoEdgesFromContour();

    ExtractTwoEdgesFromContour(PixelCurve skel, cv::Mat contour);

private:

    PixelCurve centerline_;

    cv::Mat contour_;

    std::vector<cv::Point> nonZeroIndex_;

    //- help to find the head of two edges
    //- the top two heads are the ones that
    //- is nearby the line cutting the edges
    //- neighbor shall be eliminated.
    std::vector<std::vector<double> > dist_;

    void removeHeadTail();

public:

    void extract(PixelCurve& edge1, PixelCurve& edge2);

};

#endif // EXTRACTTWOEDGESFROMCONTOUR_H
