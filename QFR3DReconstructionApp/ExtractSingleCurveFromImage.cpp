#include "ExtractSingleCurveFromImage.h"
#include <QDebug>
#include "QFRDebugger.h"

//ExtractSingleCurveFromImage::ExtractSingleCurveFromImage()
//{

//}

ExtractSingleCurveFromImage::ExtractSingleCurveFromImage(QString filename)
{
    imgOriginal_ = cv::imread(cv::String(filename.toStdString()),cv::IMREAD_GRAYSCALE);
}

ExtractSingleCurveFromImage::ExtractSingleCurveFromImage(const cv::Mat & img)
{
    img.copyTo(imgOriginal_);

    height_ = imgOriginal_.rows;

    width_ = imgOriginal_.cols;
//    imgOriginal_ = img;
}

QList<Pixel> ExtractSingleCurveFromImage::findEnds()
{
    int nrow = height_;
    int ncol = width_;
    cv::Mat &image = imgOriginal_;

//    QFRDebugger dbg;
//    dbg.display("findEnds",image);

    QList<Pixel> ends;

    for (int i = 0; i < nrow; i++)
        for (int j = 0; j < ncol; j++)
        {
            cv::Scalar intensity = image.at<uchar>(i,j);
            int grayValue = intensity.val[0];

            if (grayValue == 255)
            {
//                qDebug() << i << " " << j <<" "<< grayValue <<endl;
//
                //std::cout << "White neighbor" << std::endl;
                int count = countWhiteNeighbor(i,j);
                //std::cout << count << std::endl;
                if ( count == 1)
                {
//                    qDebug() << i << " ++++++++ " << j << endl;
                    ends.append(Pixel(i,j));
                }
            }
        }
//    qDebug() << "----------size of ends:" << ends.size() << endl;
//    qDebug() << ends[0].i() << " " << ends[0].j() << endl;
//    qDebug() << ends[1].i() << " " << ends[1].j() << endl;
    return ends;
}

int ExtractSingleCurveFromImage::countWhiteNeighbor(int i, int j)
{
    int height = height_;
    int width = width_;
    int count = 0;
    int North = i-1;
    int South = i+1;
    int West = j-1;
    int East = j+1;
    cv::Mat image;
    imgOriginal_.copyTo(image);
    if (North > 0){
        cv::Scalar intensity = image.at<uchar>(North,j);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }
    if (South < height) {
        cv::Scalar intensity = image.at<uchar>(South,j);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }
    if (West > 0) {
        cv::Scalar intensity = image.at<uchar>(i,West);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }
    if (East < width) {
        cv::Scalar intensity = image.at<uchar>(i,East);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }

    if (West > 0 && North > 0){
        cv::Scalar intensity = image.at<uchar>(North,West);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }
    if (West > 0 && South < height) {
        cv::Scalar intensity = image.at<uchar>(South,West);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }
    if (East < width && North > 0) {
        cv::Scalar intensity = image.at<uchar>(North,East);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }
    if (East < width && South < height) {
        cv::Scalar intensity = image.at<uchar>(South,East);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
            count = count + 1;
    }
//        print("count")
//        print(count)
    return count;
}

Pixel ExtractSingleCurveFromImage::checkNeighbor(int i, int j)
{
    int height = height_;
    int width = width_;
    int North = i-1;
    int South = i+1;
    int West = j-1;
    int East = j+1;
    cv::Mat &image = imgOriginal_;
    // self.imageDeepCopy[i][j] = 0
    image.at<uchar>(i,j) = 0;

    int istart = -1;
    int jstart = -1;

    if (North > 0){
        cv::Scalar intensity = image.at<uchar>(North,j);
        int grayValue = intensity.val[0];
        if (grayValue == 255) {
            istart = North;
            jstart = j;
            return Pixel(istart,jstart);
        }
    }
    if (South < height) {
        cv::Scalar intensity = image.at<uchar>(South,j);
        int grayValue = intensity.val[0];
        if (grayValue == 255)
        {
            istart = South;
            jstart = j;
            return Pixel(istart,jstart);
        }
    }
    if (West > 0) {
        cv::Scalar intensity = image.at<uchar>(i,West);
        int grayValue = intensity.val[0];
        if (grayValue == 255) {
            istart = i;
            jstart = West;
            return Pixel(istart,jstart);
        }
    }
    if (East < width) {
        cv::Scalar intensity = image.at<uchar>(i,East);
        int grayValue = intensity.val[0];
        if (grayValue == 255){
            istart = i;
            jstart = East;
            return Pixel(istart,jstart);
        }
    }

    if (West > 0 && North > 0) {
        cv::Scalar intensity = image.at<uchar>(North,West);
        int grayValue = intensity.val[0];
        if (grayValue == 255){
            istart = North;
            jstart = West;
            return Pixel(istart,jstart);
        }
    }

    if (West > 0 && South < height){
        cv::Scalar intensity = image.at<uchar>(South,West);
        int grayValue = intensity.val[0];
        if (grayValue == 255) {
            istart = South;
            jstart = West;
            return Pixel(istart,jstart);
        }
    }
    if (East < width && North > 0) {
        cv::Scalar intensity = image.at<uchar>(North,East);
        int grayValue = intensity.val[0];
        if (grayValue == 255){
            istart = North;
            jstart = East;
            return Pixel(istart,jstart);
        }
    }

    if (East < width && South < height) {
        cv::Scalar intensity = image.at<uchar>(South,East);
        int grayValue = intensity.val[0];
        if (grayValue == 255){
            istart = South;
            jstart = East;
            return Pixel(istart,jstart);
        }
    }

    return Pixel(-1,-1);
}
/*
PixelCurve ExtractSingleCurveFromImage::findCurve()
{
    QList<Pixel> curve;

    QList<Pixel> ends = findEnds();

    //- from the 1st end point
    Pixel end = ends[0];

    PixelCurve pCurve;

    pCurve.setHead(end);

    int istart = end.i();

    int jstart = end.j();

    bool stop = false;

    curve.append(Pixel(istart,jstart));

    while(!stop){

        Pixel ijstart = checkNeighbor(istart,jstart);

        istart = ijstart.i();

        jstart = ijstart.j();

//        qDebug() << istart << " , " << jstart << endl;

        if (istart == -1 && jstart == -1) {
            stop = true;
        }
        else {
            curve.append(Pixel(istart,jstart));
        }
    }

    pCurve.setTail(curve.last());

    pCurve.setCurve(curve);

    return pCurve;
}
*/
PixelCurve ExtractSingleCurveFromImage::findCurveByGivingHead(const Pixel &head)
{
    QList<Pixel> curve;

    int istart = head.i();

    int jstart = head.j();

    bool stop = false;

    curve.append(Pixel(istart,jstart));

//    QFRDebugger dbg;
//    dbg.display("findCurveByGivingEnd",imgOriginal_);

    while(!stop){

        Pixel ijstart = checkNeighbor(istart,jstart);

        istart = ijstart.i();

        jstart = ijstart.j();

        if (istart == -1 && jstart == -1) {
            stop = true;
        }
        else {
            curve.append(Pixel(istart,jstart));
        }
    }

    PixelCurve pCurve;
    pCurve.setHead(curve[0]);
    pCurve.setHeadNext(curve[1]);

    int size = curve.size();
    pCurve.setTail(curve[size-1]);
    pCurve.setTailNext(curve[size-2]);

    pCurve.setCurve(curve);

//    std::cout << "curve" << std::endl;
//    std::cout << pCurve << std::endl;
//    std::cout << curve[0] << std::endl;
//    std::cout << curve[1] << std::endl;
//    std::cout << curve[size-1] << std::endl;
//    std::cout << curve[size-2] << std::endl;
//    std::cout <<"end curve" << size << std::endl;

    return pCurve;
}
