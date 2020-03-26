#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include "DualCameraSystem2.h"
#include <QDebug>
#include "PixelUVImageXYTransformer.h"
#include "QFRDebugger.h"
#include "stl_reader.h"
#include "ValidPointDetector2.h"
#include "DualCameraSystem.h"
#include "ValidPointDetector.h"
#include <QtMath>
#include <QMessageBox>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_ReconstructionButton_clicked()
{
    // process 3D reconstruction
    for (int i = 0; i < numberOfCamera_; i++)
        for (int j = 0; j < numberOfCamera_; j++)
        {
            if (i < j)
            {
                //- allow to reload image
                //- const reference is not applied
                const CameraPinHole & cam1 = cameraList_[i];
                const CameraPinHole & cam2 = cameraList_[j];

                //- acute angel

                const double cosTheta = cam1.normal().dot(cam2.normal());
                const double angle = qRadiansToDegrees(qAcos(cosTheta));

                if (angle<=25.0){
                    QMessageBox messageBox;
                    messageBox.critical(0,"Error"," The acute angle should be at least 25 degree!");
                    messageBox.setFixedSize(500,200);
                    QApplication::quit();
                } else {
                    QMessageBox messageBox;
                    QString message = "The acute angel is "+QString::number(angle)+" degree";
                    messageBox.about(0,"Acute angel",message);
                }

                if(0) {
                    //- method-I
                    const ValidPointDetector detector(cam1,cam2);
                    DualCameraSystem dcs(detector);
                    dcs.surfaceReconstruction();

                } else {
                    //- method-II
                    const ValidPointDetector2 detector(cam1,cam2);
                    DualCameraSystem2 dcs(detector);
                    dcs.surfaceReconstruction();
                }
            }
        }
}

void MainWindow::on_LoadCameraParameter_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Camera Parameter File"), "", tr("Json Files (*.json)"));

    QFile loadFile(fileName);

    if (!loadFile.exists())
    {
        qDebug() << "NO FILE FOUND";
    }
    else
    {
        loadFile.open(QIODevice::ReadOnly);

        QByteArray jsonContext = loadFile.readAll();

        QJsonDocument dict = QJsonDocument::fromJson(jsonContext);

        QJsonObject dictObject = dict.object();

        numberOfCamera_ = dictObject["NumberOfCamera"].toInt();

        qDebug() << "number of Camera" << numberOfCamera_ << endl;

        for(int i = 0 ; i < numberOfCamera_; i++)
        {
            QString cameraKeyword = "Camera";

            cameraKeyword = cameraKeyword + QString::number(i+1);

            qDebug() << cameraKeyword << endl;

            QJsonObject cameraDictObject = dictObject[cameraKeyword].toObject();

            CameraPinHole cam(i,cameraDictObject);

            cam.print();

            cam.save();

            cameraList_.append(cam);
        }
    }
}

void MainWindow::on_CreateTwoImage_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open 3D STL File"), "", tr("STL Files (*.stl)"));

    QFile file(fileName);
    if (!file.exists()) {
                qDebug() << "NO FILE FOUND";
    } else {

        QList<Eigen::Vector3d> points;
        stl_reader::StlMesh <double, unsigned int> mesh (fileName.toStdString().c_str());
        for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
            Eigen::Vector3d p;
            //std::cout << "coordinates of triangle " << itri << ": ";
            for(size_t icorner = 0; icorner < 3; ++icorner) {
                const double* c = mesh.vrt_coords (mesh.tri_corner_ind (itri, icorner));
                //const float* c = mesh.tri_corner_coords (itri, icorner);
                //std::cout << "(" << c[0] << ", " << c[1] << ", " << c[2] << ") ";
                p(0) = c[0];
                p(1) = c[1];
                p(2) = c[2];

                points.append(p);
            }
        }

        //- compute volumetric center
        const int size = points.size();
        Eigen::Vector3d vc = Eigen::Vector3d::Zero();
        for(int i = 0; i < points.size(); i++) {
            vc = vc + points[i];
        }
        vc = vc/size;


//        solid Visualization Toolkit generated SLA File
//         facet normal 0.202112 0.212856 0.955951
//          outer loop
//           vertex 0.0341247 -0.0111384 -0.0932265
//           vertex 0.0338822 -0.0111063 -0.0931824
//           vertex 0.0339658 -0.0113221 -0.093152
//          endloop
//         endfacet
//        endsolid

        QFile stlFile( "new.stl" );
        stlFile.open(QIODevice::WriteOnly);
        QTextStream stlStream( &stlFile );
        stlStream<<"solid new stl"<<endl;

        for(size_t itri = 0; itri < mesh.num_tris(); ++itri) {
            const double* n = mesh.tri_normal(itri);
            stlStream << " facet normal " << n[0] << " " << n[1] << " " << n[2] << endl;
            stlStream << "  outer loop " << endl;
            Eigen::Vector3d p;
            //std::cout << "coordinates of triangle " << itri << ": ";
            for(size_t icorner = 0; icorner < 3; ++icorner) {
                const double* c = mesh.vrt_coords(mesh.tri_corner_ind(itri,icorner));

                //const float* c = mesh.tri_corner_coords (itri, icorner);
                //std::cout << "(" << c[0] << ", " << c[1] << ", " << c[2] << ") ";
                p(0) = c[0]-vc(0);
                p(1) = c[1]-vc(1);
                p(2) = c[2]-vc(2);

                stlStream << "   vertex " << p(0) << " " << p(1) << " " << p(2) << endl;

            }
            stlStream << "  endloop " << endl;
            stlStream << " endfacet " << endl;
        }

        stlStream << "endsolid " << endl;
        stlFile.close();

        int numberOfPoints = points.size();

        for(int i = 0; i < numberOfCamera_; i++) {
            const CameraPinHole &cam = cameraList_[i];

            int uMax = cam.resolutionU();
            int vMax = cam.resolutionV();
            PixelUVImageXYTransformer coordTransformer(uMax,vMax);
            cv::Mat img(vMax, uMax, CV_8UC1, cv::Scalar(0,0,0));
            for (int j = 0; j < numberOfPoints; j++) {
                const Eigen::Vector3d p = points[j] - vc;
                const Eigen::Vector3d x = cam.project3DTo2D(p);

                int u = coordTransformer.PixelU(x(0));
                int v = coordTransformer.PixelV(x(1));

                if (u < 0 || u >= uMax || v < 0 || v >= vMax) continue;

                img.at<uchar>(cv::Point(u,v)) = 255;
            }
            int morph_size = 10;
            cv::Mat kernel = cv::getStructuringElement( \
                        cv::MORPH_RECT, \
                        cv::Size( 2*morph_size + 1, 2*morph_size+1 ), \
                        cv::Point( morph_size, morph_size ) );
            // cv::Mat kernel(3,3,CV_8UC1, cv::Scalar(0,0,0));
            cv::morphologyEx(img,img,cv::MORPH_CLOSE,kernel);
            QFRDebugger dbg;
            dbg.display(cam.cameraName()+"_created",img);

            // cv::String imgName = QString("VesselFrom"+cam.cameraName()+".png").toStdString();
            cv::String imgName = cam.imagePath().toStdString();
            cv::imwrite(imgName,img);
        }
    }

}
