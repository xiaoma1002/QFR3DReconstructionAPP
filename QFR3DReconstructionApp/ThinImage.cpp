#include "ThinImage.h"

//ThinImage::ThinImage()
//{

//}

ThinImage::ThinImage(QString filename)
{
    imgOriginal_ = cv::imread(cv::String(filename.toStdString()),cv::IMREAD_GRAYSCALE);

    cv::threshold(imgOriginal_, imgOriginal_, 128, 1, cv::THRESH_BINARY);
}

ThinImage::ThinImage(const cv::Mat & img)
{
    img.copyTo(imgOriginal_);
//    imgOriginal_ = img;

    cv::threshold(imgOriginal_, imgOriginal_, 128, 1, cv::THRESH_BINARY);
}

cv::Mat ThinImage::skeletonization(const int maxIterations)
{
    const cv::Mat & src = imgOriginal_;
//    src = imgOriginal_;
//    cv::threshold(src, src, 128, 1, cv::THRESH_BINARY);

    //- CV_8UC1 : 1 channel
    assert(src.type() == CV_8UC1);
    cv::Mat dst;
    int width  = src.cols;
    int height = src.rows;
    // dst = src;
    src.copyTo(dst);
    int count = 0;

    while (true) {
        count++;
        if (maxIterations != -1 && count > maxIterations)
            break;
        std::vector<uchar *> mFlag;

        for (int i = 0; i < height; ++i) {
            uchar * p = dst.ptr<uchar>(i);
            for (int j = 0; j < width; ++j) {

                uchar p1 = p[j];
                if (p1 != 1) continue;
                uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
                uchar p8 = (j == 0) ? 0 : *(p + j - 1);
                uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
                uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
                uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
                uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
                uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
                uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
                if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6) {
                    int ap = 0;
                    if (p2 == 0 && p3 == 1) ++ap;
                    if (p3 == 0 && p4 == 1) ++ap;
                    if (p4 == 0 && p5 == 1) ++ap;
                    if (p5 == 0 && p6 == 1) ++ap;
                    if (p6 == 0 && p7 == 1) ++ap;
                    if (p7 == 0 && p8 == 1) ++ap;
                    if (p8 == 0 && p9 == 1) ++ap;
                    if (p9 == 0 && p2 == 1) ++ap;
                    if (ap == 1 && p2 * p4 * p6 == 0 && p4 * p6 * p8 == 0) {
                        mFlag.push_back(p+j);//
                    }
                }
            }
        }

        for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i) {
            **i = 0;
        }

        if (mFlag.empty()) {
            break;
        } else {
            mFlag.clear();
        }

        for (int i = 0; i < height; ++i) {
            uchar * p = dst.ptr<uchar>(i);
            for (int j = 0; j < width; ++j) {

                uchar p1 = p[j];
                if (p1 != 1) continue;
                uchar p4 = (j == width - 1) ? 0 : *(p + j + 1);
                uchar p8 = (j == 0) ? 0 : *(p + j - 1);
                uchar p2 = (i == 0) ? 0 : *(p - dst.step + j);
                uchar p3 = (i == 0 || j == width - 1) ? 0 : *(p - dst.step + j + 1);
                uchar p9 = (i == 0 || j == 0) ? 0 : *(p - dst.step + j - 1);
                uchar p6 = (i == height - 1) ? 0 : *(p + dst.step + j);
                uchar p5 = (i == height - 1 || j == width - 1) ? 0 : *(p + dst.step + j + 1);
                uchar p7 = (i == height - 1 || j == 0) ? 0 : *(p + dst.step + j - 1);
                if ((p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) >= 2 && (p2 + p3 + p4 + p5 + p6 + p7 + p8 + p9) <= 6) {
                    int ap = 0;
                    if (p2 == 0 && p3 == 1) ++ap;
                    if (p3 == 0 && p4 == 1) ++ap;
                    if (p4 == 0 && p5 == 1) ++ap;
                    if (p5 == 0 && p6 == 1) ++ap;
                    if (p6 == 0 && p7 == 1) ++ap;
                    if (p7 == 0 && p8 == 1) ++ap;
                    if (p8 == 0 && p9 == 1) ++ap;
                    if (p9 == 0 && p2 == 1) ++ap;
                    if (ap == 1 && p2 * p4 * p8 == 0 && p2 * p6 * p8 == 0) {
                        mFlag.push_back(p+j);
                    }
                }
            }
        }

        for (std::vector<uchar *>::iterator i = mFlag.begin(); i != mFlag.end(); ++i) {
            **i = 0;
        }

        if (mFlag.empty()) {
            break;
        } else {
            mFlag.clear();
        }
    }

    dst = dst * 255;

    //cv::cvtColor(dst,dst, cv::COLOR_GRAY2BGR);
    //cv::cvtColor(dst,dst, cv::COLOR_BGR2GRAY);

    return dst;
}
