#include "ExtractTwoEdgesFromContour.h"
#include<algorithm>
#include "SortMatrix.h"
#include "ExtractSingleCurveFromImage.h"
#include "QFRDebugger.h"

//ExtractTwoEdgesFromContour::ExtractTwoEdgesFromContour()
//{

//}

ExtractTwoEdgesFromContour::ExtractTwoEdgesFromContour(PixelCurve skel, cv::Mat contour)
{
    centerline_ = skel;

    // contour_ = contour;
    contour.copyTo(contour_);

    //- contour has to be gray image
    cv::findNonZero(contour_,nonZeroIndex_);

}

void ExtractTwoEdgesFromContour::removeHeadTail()
{
    const QList<Pixel> &centerLineSeg = centerline_.curve();
    const int size = centerLineSeg.size();
    cv::Mat & contour = contour_;
    std::vector<std::vector<double> > & dist = dist_;
    const Pixel & head = centerline_.head();
    const Pixel & nextHead = centerline_.headNext();

    const Pixel & tail = centerline_.tail();
    const Pixel & nextTail = centerline_.tailNext();

    {
        int x0[2] = {head.u(),head.v()};
        int x1[2] = {nextHead.u(), nextHead.v()};

        int x1minusX0[2] = {x1[0] - x0[0], x1[1] - x0[1]};
        double rhs = float(x1minusX0[0]*x0[0] + x1minusX0[1]*x0[1]);

        std::vector<cv::Point> whitePixels = nonZeroIndex_;
        size_t sizeOfWhitePixel = whitePixels.size();

        double line0 = x1minusX0[0]*tail.u() + x1minusX0[1]*tail.v() - rhs;

        for (int i = 0; i < sizeOfWhitePixel; i++) {
            int u = whitePixels[i].x;
            int v = whitePixels[i].y;
            double line = x1minusX0[0]*u + x1minusX0[1]*v - rhs;
            if (line*line0 <0) {
                contour.at<uchar>(cv::Point(u,v)) = 0;
            } else {
                std::vector<double> tmp;
                double distance = fabs(line);
                tmp.push_back(distance);
                tmp.push_back(i);
                dist.push_back(tmp);
            }
        }
    }

    sort(dist.begin(),dist.end(),Math::sortcol);

    {
        int x0[2] = {tail.u(), tail.v()};
        int x1[2] = {nextTail.u(),nextTail.v()};
        int x1minusX0[2] = {x1[0] - x0[0], x1[1] - x0[1]};
        double rhs = float(x1minusX0[0]*x0[0] + x1minusX0[1]*x0[1]);

        std::vector<cv::Point> whitePixels = nonZeroIndex_;
        size_t sizeOfWhitePixel = whitePixels.size();

        double line0 = x1minusX0[0]*head.u() + x1minusX0[1]*head.v() - rhs;

        for (int i = 0; i < sizeOfWhitePixel; i++) {
            int u = whitePixels[i].x;
            int v = whitePixels[i].y;
            double line = x1minusX0[0]*u + x1minusX0[1]*v - rhs;
            if (line*line0 < 0) {
                contour.at<uchar>(cv::Point(u,v)) = 0;
            }
        }
    }

//    QFRDebugger dbg;
//    dbg.display("after remove head and tail",contour);
}

void ExtractTwoEdgesFromContour::extract(PixelCurve &edge1, PixelCurve& edge2)
{
    removeHeadTail();
    std::vector<cv::Point> whitePixels = nonZeroIndex_;

    QList<Pixel> heads;
    std::vector<std::vector<double> > & dist = dist_;

//    for(int i = 0; i < dist.size(); i++){
//        qDebug() << dist[i][0] << " " << dist[i][1] << endl;
//    }

    int index = int(dist[0][1]);

    int u = whitePixels[index].x;
    int v = whitePixels[index].y;

    //- (i,j)<->(v,u)
    heads.append(Pixel(v,u));

    size_t size = dist.size();

    for (int i = 0; i < size; i++){
        int index = int(dist[i][1]);
        int u1 = whitePixels[index].x;
        int v1 = whitePixels[index].y;

        if (i == 0)
            continue;

        if (u1 == u+1 && v1 == v)
            continue;
        else if (v1 == v+1 && u1 == u)
            continue;
        else if (u1 == u-1 && v1 == v)
            continue;
        else if (v1 == v-1 && u1 == u)
            continue;
        else if (u1 == u+1 && v1 == v-1)
            continue;
        else if (u1 == u+1 && v1 == v+1)
            continue;
        else if (u1 == u-1 && v1 == v+1)
            continue;
        else if (u1 == u-1 && v1 == v-1)
            continue;
        else {
            int u = whitePixels[index].x;
            int v = whitePixels[index].y;
            heads.append(Pixel(v,u));
            break;
        }
    }

//    qDebug() << "----------size of ends(2 edges):" << heads.size() << endl;
//    qDebug() << heads[0].i() << " " << heads[0].j() << endl;
//    qDebug() << heads[1].i() << " " << heads[1].j() << endl;

    cv::Mat & contour = contour_;

//    QFRDebugger dbg;
//    dbg.display("after removing head/tail", contour);

    ExtractSingleCurveFromImage escfi1(contour);
    ExtractSingleCurveFromImage escfi2(contour);

    edge1 = escfi1.findCurveByGivingHead(heads[0]);

    edge2 = escfi2.findCurveByGivingHead(heads[1]);
}
