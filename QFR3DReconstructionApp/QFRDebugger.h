#ifndef QFRDEBUGGER_H
#define QFRDEBUGGER_H

#include <QString>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include "Pixel.h"
#include <QList>
#include "Eigen/Dense"
#include <QTextStream>
#include <QFile>

class QFRDebugger
{

public:

//    QFRDebugger();

    void display(QString winTitle, const cv::Mat & img);

    void display(QString winTitle, const cv::Mat &orgImg, const QList<Pixel> &p);

    void displayOverlay(QString winTitle, const cv::Mat &orgImg, const QList<Pixel> &p);

    void saveFile(Eigen::MatrixXd, QString filename);
};

#endif // QFRDEBUGGER_H
