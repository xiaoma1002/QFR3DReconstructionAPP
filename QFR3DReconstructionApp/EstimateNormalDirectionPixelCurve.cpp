#include "EstimateNormalDirectionPixelCurve.h"
#include "Pixel.h"
#include <QMessageBox>
#include <QApplication>

EstimateNormalDirectionPixelCurve::EstimateNormalDirectionPixelCurve(const PixelCurve &curve):
    curve_(curve)
{
    stride_ = 1;

    if (curve_.curve().size() < 2*stride_) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," the curve is too short!");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }

    estimate();
}

void EstimateNormalDirectionPixelCurve::estimate()
{
    const QList<Pixel>& curve = curve_.curve();
    const int size = curve.size();

    for (int i = 0; i < size; i++) {
        Eigen::Vector2d normal,orthNormal;
        findDirection(i,normal,orthNormal);
        direction_.append(normal);
        orthNormal_.append(orthNormal);
    }
}

void EstimateNormalDirectionPixelCurve::findDirection(\
        int it, \
        Eigen::Vector2d& direction,\
        Eigen::Vector2d& orthNormal)
{
    const QList<Pixel>& curve = curve_.curve();
    const int size = curve.size();

    bool found = false;
    int headIndex = 0;
    int tailIndex = 0;

    while (!found) {
        headIndex = it - stride();
        tailIndex = it + stride();

        if (headIndex < 0) {
            headIndex = 0;
            tailIndex = tailIndex - headIndex;
        } else if (tailIndex > size-1) {
            headIndex = headIndex - (tailIndex- size + 1);
            tailIndex = size-1;
        }

        const Pixel & pHead = curve[headIndex];
        const Pixel & pTail = curve[tailIndex];

        if (pHead.u() != pTail.u() && pHead.v() != pTail.v()) {
                found = true;
        } else {
            //- redo
            setStride(stride()+1);
        }
    }

    if (found) {
        const Pixel & pHead = curve[headIndex];
        const Pixel & pTail = curve[tailIndex];

        double uHead = pHead.u();
        double vHead = pHead.v();
        double uTail = pTail.u();
        double vTail = pTail.v();
        Eigen::Vector2d Head, Tail;
        Head << uHead,vHead;
        Tail << uTail, vTail;
        direction = Tail - Head;
        double length = direction.norm();
        if (length == 0) {
            QMessageBox messageBox;
            messageBox.critical(0,"Error"," zero length normal!");
            messageBox.setFixedSize(500,200);
            QApplication::quit();
        }
        direction = direction/length;

        orthNormal(0) = 1.0;
        orthNormal(1) = -direction(0)*orthNormal(0)/direction(1);

        length = orthNormal.norm();
        orthNormal = orthNormal/length;

    }
}

void EstimateNormalDirectionPixelCurve::setStride(const int stride)
{
    stride_ = stride;
}

std::ostream& operator<<(std::ostream& os, const EstimateNormalDirectionPixelCurve& endpc)
{
    const int size = endpc.direction().size();
    for(int i = 0; i < size; i++) {
        const Eigen::Vector2d & direction = endpc.direction()[i];
        os << "[" << direction(0) << ',' << direction(1) << "]" << std::endl;
    }

    return os;
}
