#include "PixelCurve.h"
#include <QApplication>
#include <QMessageBox>

PixelCurve::PixelCurve()
{
    head_ = Pixel(-1,-1);
    headNext_ = Pixel(-1,-1);
    tail_ = Pixel(-1,-1);
    tailNext_ = Pixel(-1,-1);
}

void PixelCurve::setCurve(const QList<Pixel> &curve)
{
    curve_ = curve;
}

void PixelCurve::setHead(const Pixel &head)
{
    head_ = head;
}

void PixelCurve::setHeadNext(const Pixel &headNext)
{
    headNext_ = headNext;
}

void PixelCurve::setTail(const Pixel &tail)
{
    tail_ = tail;
}

void PixelCurve::setTailNext(const Pixel &tailNext)
{
    tailNext_ = tailNext;
}

const QList<Pixel> &PixelCurve::curve() const
{
    if (curve_.size()<2) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," Invalid curve in Image !");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }
    return curve_;
}

const Pixel &PixelCurve::head() const
{
    if (head_.i() < 0) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," Invalid Head in PixelCurve !");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }

    return head_;
}

const Pixel &PixelCurve::headNext() const
{
    if (headNext_.i() < 0) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," Invalid headNext in PixelCurve !");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }

    return headNext_;
}

const Pixel &PixelCurve::tail() const
{
    if (tail_.i() < 0) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," Invalid Tail in PixelCurve !");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }

    return tail_;
}

const Pixel &PixelCurve::tailNext() const
{
    if (tailNext_.i() < 0) {
        QMessageBox messageBox;
        messageBox.critical(0,"Error"," Invalid tailNext in PixelCurve !");
        messageBox.setFixedSize(500,200);
        QApplication::quit();
    }

    return tailNext_;
}

PixelCurve &PixelCurve::operator=(const PixelCurve &p)
{
    head_ = p.head();
    tail_ = p.tail();
    headNext_ = p.headNext();
    tailNext_ = p.tailNext();
    curve_ = p.curve();

    return *this;
}

std::ostream &operator<<(std::ostream &os, const PixelCurve &pixelCurve)
{

    os << "head: " << pixelCurve.head() << std::endl;
    os << "headNext: " << pixelCurve.headNext() << std::endl;
    os << "tail: " << pixelCurve.tail() << std::endl;
    os << "tailNext: " << pixelCurve.tailNext() << std::endl;

    os << "size: " << pixelCurve.curve().size() << std::endl;

    const QList<Pixel> &curve = pixelCurve.curve();
    const int size = curve.size();
    for (int i = 0; i < size; i++) {
        os << "curve ith pixel : " << curve[i] << std::endl;
    }

    return os;
}
