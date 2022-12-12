#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <algorithm>
#include <QVector>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    int x1, x2;
    QString line;
    line = ui->lineEdit_2->text();
    x1 = line.toInt();
    line = ui->lineEdit_3->text();
    x2 = line.toInt();
    ui->widget->xAxis->setRange(x1, x2);
    line = ui->lineEdit_4->text();
    x1 = line.toInt();
    line = ui->lineEdit_5->text();
    x2 = line.toInt();
    ui->widget->yAxis->setRange(x1, x2);
    ui->widget->xAxis->setLabel("x");
    ui->widget->yAxis->setLabel("y");
    ui->widget->setInteraction(QCP::iRangeDrag, true);
    ui->widget->setInteraction(QCP::iRangeZoom, true);
    for (int i = 0; i < 13; i++)
        ui->widget->addGraph();
}

MainWindow::~MainWindow()
{
    delete ui;
}

QTime fromMS(quint64 totalMS )
{
    uint h = totalMS / 1000 / 60 / 60 % 60;
    uint min = totalMS / 1000 / 60 % 60;
    uint s = totalMS / 1000 % 60;
    uint ms = totalMS % 1000;
    return QTime(h, min, s, ms);
}

void MainWindow::on_pushButton_clicked()
{
    QTime t1 = QTime::currentTime();
    int xBegin, xEnd, yBegin, yEnd;
    double delay;
    QString line;
    line = ui->lineEdit_8->text();
    xBegin = line.toInt();
    line = ui->lineEdit_7->text();
    xEnd = line.toInt();
    line = ui->lineEdit_9->text();
    yBegin = line.toInt();
    line = ui->lineEdit_6->text();
    yEnd = line.toInt();
    line = ui->lineEdit->text();
    delay = line.toDouble();
    double k, b;
    if (xBegin > xEnd)
    {
        std::swap(xBegin, xEnd);
        std::swap(yBegin, yEnd);
    }
    if (xEnd - xBegin == 0)
    {
        ui->widget->graph(0)->addData(xBegin, yBegin);
        ui->widget->replot();
        ui->widget->graph(0)->addData(xEnd, yEnd);
        ui->widget->replot();
    }
    else
    {
        k = 1.0 * (yEnd - yBegin) / (xEnd - xBegin);
        b = yEnd - k * xEnd;
        ui->label_17->setNum(xBegin);
        ui->label_19->setNum(yBegin);
        ui->widget->graph(0)->addData(xBegin, yBegin);
        ui->widget->replot();
        QThread::msleep(delay);
        while(xBegin < xEnd)
        {
            xBegin++;
            yBegin = round(k * xBegin + b);
            ui->label_17->setNum(xBegin);
            ui->label_19->setNum(yBegin);
            ui->widget->graph(0)->addData(xBegin, yBegin);
            ui->widget->replot();
            QThread::msleep(delay);
        }
    }
    ui->widget->replot();
    ui->label_17->setText("");
    ui->label_19->setText("");
    quint64 totalMS = qAbs(QTime::currentTime().msecsTo(t1));
    QTime t2 = fromMS(totalMS);
    qDebug() << "Для работы пошагового алгоритма потребовалось времени:" << t2;
}

void MainWindow::on_pushButton_2_clicked()
{
    QTime t1 = QTime::currentTime();
    int xBegin, xEnd, yBegin, yEnd;
    double delay;
    QString line;
    line = ui->lineEdit_8->text();
    xBegin = line.toInt();
    line = ui->lineEdit_7->text();
    xEnd = line.toInt();
    line = ui->lineEdit_9->text();
    yBegin = line.toInt();
    line = ui->lineEdit_6->text();
    yEnd = line.toInt();
    line = ui->lineEdit->text();
    delay = line.toDouble();
    int dx = xEnd - xBegin;
    int dy = yEnd - yBegin;
    int step;
    if (abs(dx) > abs(dy))
        step = abs(dx);
    else
        step = abs(dy);
    float x_incr = (float)dx / step;
    float y_incr = (float)dy / step;
    float x = xBegin;
    float y = yBegin;
    for (int i = 0; i <= step; i++)
    {
        ui->label_17->setNum(x);
        ui->label_19->setNum(y);
        ui->widget->graph(2)->addData(x,y);
        ui->widget->replot();
        QThread::msleep(delay);
        x += x_incr;
        y += y_incr;
    }
    ui->label_17->setText("");
    ui->label_19->setText("");
    quint64 totalMS = qAbs(QTime::currentTime().msecsTo(t1));
    QTime t2 = fromMS(totalMS);
    qDebug() << "Для работы алгоритма ЦДА потребовалось времени:" << t2;
}

void MainWindow::on_pushButton_3_clicked()
{
    QTime t1 = QTime::currentTime();
    int xBegin, xEnd, yBegin, yEnd;
    double delay;
    QString line;
    line = ui->lineEdit_8->text();
    xBegin = line.toInt();
    line = ui->lineEdit_7->text();
    xEnd = line.toInt();
    line = ui->lineEdit_9->text();
    yBegin = line.toInt();
    line = ui->lineEdit_6->text();
    yEnd = line.toInt();
    line = ui->lineEdit->text();
    delay = line.toDouble();
    if(abs(yEnd - yBegin) > abs(xEnd - xBegin))
    {
       std::swap(xBegin, yBegin);
       std::swap(xEnd, yEnd);
    }
    if (xBegin > xEnd)
    {
        std::swap(xBegin, xEnd);
        std::swap(yBegin, yEnd);
    }
    int Dx = xEnd-xBegin;
    int Dy = yEnd-yBegin;
    int yi = 1;
    if (Dy < 0)
    {
        yi = -1;
        Dy = -Dy;
    }
    int D = 2 * Dy - Dx;
    int y = yBegin;
    for (int x = xBegin; x <= xEnd; x++)
    {
        ui->label_17->setNum(x);
        ui->label_19->setNum(y);
        ui->widget->graph(1)->addData(x,y);
        ui->widget->replot();
        QThread::msleep(delay);
        if (D > 0)
        {
            y += yi;
            D += 2 * (Dy - Dx);
        }
        else
            D += 2 * Dy;
    }
    ui->label_17->setText("");
    ui->label_19->setText("");
    quint64 totalMS = qAbs(QTime::currentTime().msecsTo(t1));
    QTime t2 = fromMS(totalMS);
    qDebug() << "Для работы алгоритма Брезенхема потребовалось времени:" << t2;
}

void MainWindow::on_pushButton_4_clicked()
{
    QTime t1 = QTime::currentTime();
    int xm = 0, ym = 0, r = 3, delay;
    QString line;
    line = ui->lineEdit_10->text();
    xm = line.toInt();
    line = ui->lineEdit_12->text();
    ym = line.toInt();
    line = ui->lineEdit_11->text();
    r = line.toInt();
    line = ui->lineEdit->text();
    delay = line.toInt();
    int x = -r, y = 0, err = 2 - 2 * r;
    while (x < 0)
    {
        r = err;
        if (r <= y)
            err += ++y * 2 + 1;
        if (r > x || err > y)
            err += ++x * 2 + 1;
        ui->label_17->setNum(x);
        ui->label_19->setNum(y);
        ui->widget->graph(3)->addData(xm+x, ym-y);
        ui->widget->replot();
        ui->widget->graph(4)->addData(xm+y, ym-x);
        ui->widget->replot();
        ui->widget->graph(5)->addData(xm+y, ym+x);
        ui->widget->replot();
        ui->widget->graph(6)->addData(xm+x, ym+y);
        ui->widget->replot();
        ui->widget->graph(7)->addData(xm-x, ym-y);
        ui->widget->replot();
        ui->widget->graph(8)->addData(xm-y, ym-x);
        ui->widget->replot();
        ui->widget->graph(9)->addData(xm-y, ym+x);
        ui->widget->replot();
        ui->widget->graph(10)->addData(xm-x, ym+y);
        ui->widget->replot();
        QThread::msleep(delay);
    }
    ui->label_17->setText("");
    ui->label_19->setText("");
    quint64 totalMS = qAbs(QTime::currentTime().msecsTo(t1));
    QTime t2 = fromMS(totalMS);
    qDebug() << "Для работы алгоритма Брезенхема (окружность) потребовалось времени:" << t2;
}

void MainWindow::on_pushButton_5_clicked()
{
    ui->widget->graph(0)->data().data()->clear();
    ui->widget->replot();
}

void MainWindow::on_pushButton_6_clicked()
{
    Bresenham = QColorDialog::getColor(Bresenham, this);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(Bresenham);
    ui->widget->graph(1)->setPen(pen);
}

void MainWindow::on_pushButton_7_clicked()
{
    ui->widget->graph(2)->data().data()->clear();
    ui->widget->replot();
}

void MainWindow::on_pushButton_8_clicked()
{
    ui->widget->graph(3)->data().data()->clear();
    ui->widget->graph(4)->data().data()->clear();
    ui->widget->graph(5)->data().data()->clear();
    ui->widget->graph(6)->data().data()->clear();
    ui->widget->graph(7)->data().data()->clear();
    ui->widget->graph(8)->data().data()->clear();
    ui->widget->graph(9)->data().data()->clear();
    ui->widget->graph(10)->data().data()->clear();
    ui->widget->replot();
}

void MainWindow::on_pushButton_9_clicked()
{
    StepByStep = QColorDialog::getColor(StepByStep, this);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(StepByStep);
    ui->widget->graph(0)->setPen(pen);
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->widget->graph(1)->data().data()->clear();
    ui->widget->replot();
}

void MainWindow::on_pushButton_11_clicked()
{
    DDA = QColorDialog::getColor(DDA, this );
    QPen pen;
    pen.setWidth(1);
    pen.setColor(DDA);
    ui->widget->graph(2)->setPen(pen);
}

void MainWindow::on_pushButton_12_clicked()
{
    BresenhamCircle = QColorDialog::getColor(BresenhamCircle, this);
    QPen pen;
    pen.setWidth(1);
    pen.setColor(BresenhamCircle);
    ui->widget->graph(3)->setPen(pen);
}

void MainWindow::on_pushButton_13_clicked()
{
    int x1, x2;
    QString line;
    line = ui->lineEdit_2->text();
    x1 = line.toInt();
    line = ui->lineEdit_3->text();
    x2 = line.toInt();
    ui->widget->xAxis->setRange(x1, x2);
    line = ui->lineEdit_4->text();
    x1 = line.toInt();
    line = ui->lineEdit_5->text();
    x2 = line.toInt();
    ui->widget->yAxis->setRange(x1, x2);
    ui->widget->replot();
}
