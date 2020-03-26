#include "ContourDetector.h"
#include <QDebug>
#include "QFRDebugger.h"

using namespace cv;
using namespace std;

//ContourDetector::ContourDetector()
//{

//}

ContourDetector::ContourDetector(QString filename)
{
    imgOriginal_ = cv::imread(cv::String(filename.toStdString()),cv::IMREAD_GRAYSCALE);
}

ContourDetector::ContourDetector(const cv::Mat & img)
{
    img.copyTo(imgOriginal_);
}

cv::Mat ContourDetector::findContour()
{
    int thresh = 100;
    //- Convert image to gray and blur it
    cv::Mat src = imgOriginal_;
    cv::Mat src_gray;

    //- 1 channel to 3
    Mat dst;
    cvtColor(src,dst,cv::COLOR_GRAY2BGR);

    //- 3 channel to 1
    cvtColor(dst, src_gray, cv::COLOR_BGR2GRAY);
    blur(src_gray, src_gray, cv::Size(3, 3));

    //- Detect edges using Canny
    cv::Mat canny_output;
    cv::Canny(src_gray, canny_output, thresh, thresh * 2);

    //- Find contours
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny_output, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE);

    //- Draw contours
    Mat drawing = Mat::zeros(canny_output.size(), CV_8UC3);
    for (size_t i = 0; i < contours.size(); i++)
    {
        // Scalar color = Scalar(rng.uniform(0, 256), rng.uniform(0, 256), rng.uniform(0, 256));
        Scalar color = Scalar(255,255,255); // white
        drawContours(drawing, contours, (int)i, color, 1, LINE_8, hierarchy, 0);
    }

    //- convert to gray image
    cv::cvtColor(drawing, drawing, cv::COLOR_BGR2GRAY);

    return drawing;
}
