#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "CameraPinHole.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:

    int numberOfCamera_;

    QList<CameraPinHole> cameraList_;

private slots:

    void on_ReconstructionButton_clicked();

    void on_LoadCameraParameter_clicked();

    void on_CreateTwoImage_clicked();
    
private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
