#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    const QSize size = QSize(170, 100);
    openImageButton = new QPushButton("Выбрать изображение");
    histogramButton = new QPushButton("Отобразить гистограмму");
    openImageButton->setFixedSize(size);
    histogramButton->setFixedSize(size);
    imageLabel = new QLabel();
    vLayout = new QVBoxLayout();
    hLayout = new QHBoxLayout();
    vLayout->addWidget(openImageButton);
    vLayout->addWidget(histogramButton);
    hLayout->addLayout(vLayout);
    hLayout->addWidget(imageLabel);
    QWidget* widget = new QWidget();
    widget->setLayout(hLayout);
    this->setCentralWidget(widget);
    connect(this->openImageButton, SIGNAL(clicked()), SLOT(openFile()));
    connect(this->histogramButton, SIGNAL(clicked()), SLOT(histogramBuild()));
    this->setFixedHeight(900);
    this->setFixedWidth(900);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openFile()
{
    path = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "D:/", "All Files(*.*);; JPEG Image(*.jpg);; PNG Image(*.png);");
    QImage img(path);
    imageLabel->setPixmap(QPixmap::fromImage(img.scaled(700, 700)));
}

void MainWindow::histogramBuild()
{
    Mat img = cv::imread(path.toUtf8().data());
    if(!img.empty())
    {
        std::vector<cv::Mat> bgrPlanes;
        split(img, bgrPlanes);

        int histogramSize = 256;
        float range[] = { 0, 256 };
        const float* histogramRange[] = { range };
        bool uniform = true, accumulate = false;

        cv::Mat bHist, gHist, rHist;
        calcHist(&bgrPlanes[0], 1, 0, cv::Mat(), bHist, 1, &histogramSize, histogramRange, uniform, accumulate);
        calcHist(&bgrPlanes[1], 1, 0, cv::Mat(), gHist, 1, &histogramSize, histogramRange, uniform, accumulate);
        calcHist(&bgrPlanes[2], 1, 0, cv::Mat(), rHist, 1, &histogramSize, histogramRange, uniform, accumulate);

        int histogramWidth = 512, histogramHeight = 400;
        int bin_w = cvRound((double)histogramWidth / histogramSize);

        cv::Mat histImage(histogramHeight, histogramWidth, CV_8UC3, cv::Scalar(0,0,0));
        normalize(bHist, bHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
        normalize(gHist, gHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
        normalize(rHist, rHist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
        for (int i = 1; i < histogramSize; i++)
        {
            line(histImage, cv::Point(bin_w * (i - 1), histogramHeight - cvRound(bHist.at<float>(i - 1))),
                 cv::Point(bin_w * (i), histogramHeight - cvRound(bHist.at<float>(i))),
                 cv::Scalar(255, 0, 0), 2, 8, 0);
            line(histImage, cv::Point(bin_w * (i - 1), histogramHeight - cvRound(gHist.at<float>(i - 1))),
                  cv::Point(bin_w * (i), histogramHeight - cvRound(gHist.at<float>(i))),
                  cv::Scalar(0, 255, 0), 2, 8, 0 );
            line(histImage, cv::Point(bin_w * (i - 1), histogramHeight - cvRound(rHist.at<float>(i - 1))),
                  cv::Point(bin_w * (i), histogramHeight - cvRound(rHist.at<float>(i))),
                  cv::Scalar(0, 0, 255), 2, 8, 0);
        }
        imshow("calcHist Demo", histImage);
    }
}
