#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QApplication>
#include <QMainWindow>
#include <QFileDialog>
#include <QLabel>
#include <QDebug>
#include <QPixmap>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include "opencv2/objdetect/objdetect.hpp"
#include <Windows.h>
#include <iostream>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

using namespace std;
using namespace cv;

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void openFile();
    void histogramBuild();
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Ui::MainWindow *ui;
    QString path = "";
    QLabel* imageLabel;
    QPushButton* histogramButton;
    QPushButton* filterButton;
    QPushButton* openImageButton;
    QVBoxLayout* vLayout;
    QHBoxLayout* hLayout;
};
#endif // MAINWINDOW_H
