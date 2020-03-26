#ifndef EXTRACTCURVEFROMIMAGE_H
#define EXTRACTCURVEFROMIMAGE_H

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <QString>
#include <QList>
#include "Pixel.h"
#include "PixelCurve.h"

class ExtractSingleCurveFromImage
{

public:

//    ExtractSingleCurveFromImage();

    ExtractSingleCurveFromImage(QString filename);

    ExtractSingleCurveFromImage(const cv::Mat&);

private:

    cv::Mat imgOriginal_;

    Pixel start_;

    Pixel end_;

    QList<Pixel> ends_;

    int height_;

    int width_;

    //- member functions

        int countWhiteNeighbor(int i, int j);

        Pixel checkNeighbor(int i, int j);

public:
    //- edit

        QList<Pixel> findEnds();

        // PixelCurve findCurve();

        PixelCurve findCurveByGivingHead(const Pixel&head);

};

#endif // EXTRACTCURVEFROMIMAGE_H
