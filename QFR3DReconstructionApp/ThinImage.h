#ifndef THINIMAGE_H
#define THINIMAGE_H

#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <iostream>
#include <vector>
#include <QString>

class ThinImage
{
public:

//    ThinImage();

    ThinImage(QString filename);

    ThinImage(const cv::Mat&);

private:

    cv::Mat imgOriginal_;

public:

    //- edit

        cv::Mat skeletonization(const int maxIterations = -1);

};

#endif // THINIMAGE_H
