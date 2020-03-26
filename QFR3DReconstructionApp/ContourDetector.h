#ifndef CONTOURDETECTOR_H
#define CONTOURDETECTOR_H

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <QString>

class ContourDetector
{

public:

//    ContourDetector();

    ContourDetector(QString filename);

    ContourDetector(const cv::Mat&);

private:

    cv::Mat imgOriginal_;

public:

    //- edit

        cv::Mat findContour();
};

#endif // CONTOURDETECTOR_H
