#include "QFRDebugger.h"
#include <QList>

//QFRDebugger::QFRDebugger()
//{

//}

void QFRDebugger::display(QString winTitle, const cv::Mat &img)
{
    //const char * winName = winTitle.toLocal8Bit().data();
    cv::String winName = winTitle.toStdString();

    cv::namedWindow(winName,cv::WINDOW_NORMAL);

    cv::imshow(winName, img);

    cv::resizeWindow(winName,600,600);

    cv::waitKey(0);

    cv::imwrite(winName+".png",img);

    cv::destroyWindow(winName);
}

void QFRDebugger::display(QString winTitle, const cv::Mat &orgImg, const QList<Pixel> &p)
{
    int height = orgImg.rows;
    int width = orgImg.cols;
    cv::Mat img(height, width, CV_8UC3, cv::Scalar(0,0,0));

    int size = p.size();
    for (int k = 0; k < size; k++)
    {
        int i = p[k].i();
        int j = p[k].j();
        cv::Vec3b & p = img.at<cv::Vec3b>(i,j);
        p[0] = 255;
        p[1] = 0;
        p[2] = 255;
        if (k==0) {
            //img.at<uchar>(i,j) = 255;
        }
    }

    display(winTitle,img);
}

void QFRDebugger::displayOverlay(QString winTitle, const cv::Mat &orgImg, const QList<Pixel> &p)
{
    cv::Mat img;
    orgImg.copyTo(img);
    cv::cvtColor(img,img,cv::COLOR_GRAY2RGB);
    int size = p.size();
    for (int k = 0; k < size; k++)
    {
        int i = p[k].i();
        int j = p[k].j();

        if (i>=500 || j >=500){
            //qDebug() << i << " " << j;
            continue;
        }
        cv::Vec3b & p = img.at<cv::Vec3b>(i,j);
        p[0] = 255;
        p[1] = 0;
        p[2] = 255;
    }

    display(winTitle,img);
}

void QFRDebugger::saveFile(Eigen::MatrixXd m, QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QTextStream stream( &file );

    int nrow = m.rows();
    int ncol = m.cols();

    stream <<"[";
    for (int i = 0; i < nrow; i++) {
        stream <<"[ ";
        for (int j = 0; j < ncol; j++){
            stream << m(i,j);
            if (j < ncol-1)
                stream << ", ";
        }
        stream <<"]";
        if (i < nrow-1)
            stream << ", "<< endl;
    }
    stream <<"]";

    file.close();
}
