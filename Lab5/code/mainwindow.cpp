#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFile>
#include <QFileDialog>
#include <iostream>
#include <qwt_plot.h>
#include <qwt_plot_grid.h>
#include <qwt_legend.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>
#include <qwt_plot_magnifier.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_picker.h>
#include <qwt_picker_machine.h>
#include <qwt_plot_dict.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->widget->setAxisTitle(QwtPlot::xBottom, "X");
    ui->widget->setAxisTitle(QwtPlot::yLeft, "Y");
    ui->widget->setAxisScale(QwtPlot::xBottom, 0, 20);
    ui->widget->setAxisScale(QwtPlot::yLeft, 0, 20);
    QwtPlotGrid *grid = new QwtPlotGrid();
    grid->setMajorPen(QPen(Qt::gray, 1));
    grid->attach(ui->widget);
    QwtPlotMagnifier *magnifier = new QwtPlotMagnifier(ui->widget->canvas());
    magnifier->setMouseButton(Qt::MidButton);
    QwtPlotPanner *d_panner = new QwtPlotPanner(ui->widget->canvas());
    d_panner->setMouseButton(Qt::RightButton);
    QwtPlotPicker *d_picker = new QwtPlotPicker(QwtPlot::xBottom, QwtPlot::yLeft, QwtPlotPicker::CrossRubberBand,
                                                QwtPicker::ActiveOnly, ui->widget->canvas());
    d_picker->setRubberBandPen(QColor(Qt::red));
    d_picker->setTrackerPen(QColor(Qt::black));
    d_picker->setStateMachine(new QwtPickerDragPointMachine());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::fill(QPointF begin, QPointF end)
{
    QwtPlotCurve *curve = new QwtPlotCurve();
    QPolygonF points(2);
    points[0] = begin;
    points[1] = end;
    curve->setSamples(points);
    v.push_back(curve);
    v.last()->attach(ui->widget);
}

void MainWindow::midpoint(QPointF begin, QPointF end)
{
    if (end.rx() - begin.rx() == 0 && end.ry() - begin.ry() != 0)
    {
        if (abs(begin.ry() - end.ry()) < 0.1)
            return;
        if (begin.rx() > maxPolygon.rx() || begin.rx() < minPolygon.rx() ||
                (begin.ry() > maxPolygon.ry() && end.ry() > maxPolygon.ry()) ||
                (begin.ry() < minPolygon.ry() && end.ry() < minPolygon.ry())) {
            return;
        }
        if ((begin.rx() >= minPolygon.rx()) && (begin.rx() <= maxPolygon.rx()) &&
                (end.rx() >= minPolygon.rx()) && (end.rx() <= maxPolygon.rx()) &&
                (begin.ry() >= minPolygon.ry()) && (begin.ry() <= maxPolygon.ry()) &&
                (end.ry() >= minPolygon.ry()) && (end.ry() <= maxPolygon.ry())) {
            QPolygonF points(2);
            points[0] = begin;
            points[1] = end;
            QwtPlotCurve *curve = new QwtPlotCurve();
            curve->setPen(Qt::blue, 1);
            curve->setSamples(points);
            curve->attach(ui->widget);
            ui->widget->replot();
            return;
        }
        midpoint(begin, QPointF((begin.rx() + end.rx()) / 2.0, (begin.ry() + end.ry()) / 2.0));
        midpoint(QPointF((begin.rx() + end.rx()) / 2.0, (begin.ry() + end.ry()) / 2.0), end);
    }
    if (begin.rx() > end.rx())
    {
        int tmp;
        tmp = begin.rx();
        begin.setX(end.rx());
        end.setX(tmp);
        tmp = begin.ry();
        begin.setY(end.ry());
        end.setY(tmp);
    }
    double k, b;
    k = 1.0 * (end.ry() - begin.ry()) / (end.rx() - begin.rx());
    b = end.ry() - k * end.rx();
    if (sqrt((begin.rx() - end.rx()) * (begin.rx() - end.rx()) + (begin.ry() - end.ry()) * (begin.ry() - end.ry())) < 0.1)
        return;
    if ((begin.rx() < minPolygon.rx() &&
         ((k * minPolygon.rx() + b) < minPolygon.ry() || (k * minPolygon.rx() + b) > maxPolygon.ry())) ||
            (end.rx() > maxPolygon.rx() &&
             ((k * maxPolygon.rx() + b) < minPolygon.ry() || (k * maxPolygon.rx() + b) > maxPolygon.ry())) ||
            (begin.ry() > maxPolygon.ry() &&
             ((maxPolygon.ry() - b) / k < minPolygon.rx() || (maxPolygon.ry() - b) / k > maxPolygon.rx())) ||
            (begin.ry() < minPolygon.ry() &&
             ((minPolygon.ry() - b) / k < minPolygon.rx() || (minPolygon.ry() - b) / k > maxPolygon.rx()))) {
        return;
    }
    if((begin.rx() >= minPolygon.rx()) && (begin.rx() <= maxPolygon.rx()) &&
            (end.rx() >= minPolygon.rx()) && (end.rx() <= maxPolygon.rx()) &&
            (begin.ry() >= minPolygon.ry()) && (begin.ry() <= maxPolygon.ry()) &&
            (end.ry() >= minPolygon.ry()) && (end.ry() <= maxPolygon.ry())) {
        QPolygonF points(2);
        points[0] = begin;
        points[1] = end;
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setPen(Qt::blue, 1);
        curve->setSamples(points);
        curve->attach(ui->widget);
        ui->widget->replot();
        return;
    }
    midpoint(begin, QPointF((begin.rx() + end.rx()) / 2.0, (begin.ry() + end.ry()) / 2.0));
    midpoint(QPointF((begin.rx() + end.rx()) / 2.0, (begin.ry() + end.ry()) / 2.0), end);
}

void MainWindow::Polygon(QPointF begin, QPointF end)
{
    ui->widget->insertLegend(new QwtLegend());
    if (begin.rx() > end.rx())
    {
        int tmp;
        tmp = begin.rx();
        begin.setX(end.rx());
        end.setX(tmp);
        tmp = begin.ry();
        begin.setY(end.ry());
        end.setY(tmp);
    }
    double k, b;
    if (end.rx() - begin.rx() == 0 && end.ry() - begin.ry() != 0)
    {
        QPolygonF points(2);
        if (begin.rx() <= maxPolygon.rx() && begin.rx() >= minPolygon.rx())
        {
            if (begin.ry() >= minPolygon.ry() && begin.ry() <= maxPolygon.ry())
            {
                points[0] = begin;
                if(end.ry() > maxPolygon.ry())
                    points[1] = QPointF(begin.rx(), maxPolygon.ry());
                else if(end.ry() < minPolygon.ry())
                    points[1] = QPointF(begin.rx(), minPolygon.ry());
                else
                    points[1] = end;
            }
            else if (begin.ry() < minPolygon.ry() && end.ry() <= maxPolygon.ry() && end.ry() >= minPolygon.ry())
            {
                points[0] = QPointF(begin.rx(), minPolygon.ry());
                points[1] = end;
            }
            else if (begin.ry() < minPolygon.ry() && end.ry() > maxPolygon.ry())
            {
                points[0] = QPointF(begin.rx(), minPolygon.ry());
                points[1] = QPointF(begin.rx(), maxPolygon.ry());
            }
            else if (begin.ry()>maxPolygon.ry() && end.ry() >= minPolygon.ry() && end.ry() <= maxPolygon.ry())
            {
                points[0] = QPointF(begin.rx(), maxPolygon.ry());
                points[1] = end;
            }
            else if (begin.ry() > maxPolygon.ry() && end.ry() < minPolygon.ry())
            {
                points[0] = QPointF(begin.rx(), maxPolygon.ry());
                points[1] = QPointF(begin.rx(), minPolygon.ry());
            }
            QwtPlotCurve *curve = new QwtPlotCurve();
            curve->setTitle("Отсеченный отрезок");
            curve->setPen(Qt::blue, 1);
            coordX.push_back(points[0]);
            coordY.push_back(points[1]);
            curve->setSamples( points );
            curve->attach( ui->widget );
            ui->widget->replot();
            return;
        }
        return;
    }
    k = 1.0 * (end.ry() - begin.ry()) / (end.rx() - begin.rx());
    b = end.ry() - k * end.rx();
    if (begin.rx() >= minPolygon.rx() && begin.rx()<=maxPolygon.rx() && end.rx() >= minPolygon.rx() &&
            end.rx() <= maxPolygon.rx() && begin.ry() >= minPolygon.ry() && begin.ry() <= maxPolygon.ry() &&
            end.ry() >= minPolygon.ry() && end.ry() <= maxPolygon.ry()) {
        QPolygonF points(2);
        points[0] = begin;
        points[1] = end;
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setTitle("Отсеченный отрезок");
        curve->setPen(Qt::blue, 1);
        coordX.push_back(points[0]);
        coordY.push_back(points[1]);
        curve->setSamples( points );
        curve->attach(ui->widget);
        ui->widget->replot();
        return;
    }
    if (begin.rx() <= maxPolygon.rx() && begin.rx() >= minPolygon.rx() &&
            begin.ry() <= maxPolygon.ry() && begin.ry() >= minPolygon.ry()) {
        QPolygonF points(2);
        points[0] = begin;
        if (end.rx() <= maxPolygon.rx() && end.ry() > maxPolygon.ry())
            points[1] = QPointF((maxPolygon.ry() - b) / k, maxPolygon.ry());
        else if (end.rx() <= maxPolygon.rx() && end.ry() < minPolygon.ry())
             points[1] = QPointF((minPolygon.ry() - b) / k, minPolygon.ry());
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setTitle("Отсеченный отрезок");
        curve->setPen(Qt::blue, 1);
        coordX.push_back(points[0]);
        coordY.push_back(points[1]);
        curve->setSamples(points);
        curve->attach(ui->widget);
        ui->widget->replot();
        return;
    }
    if ((begin.rx() < minPolygon.rx()) && ((k*minPolygon.rx() + b) >= minPolygon.ry()) &&
            ((k * minPolygon.rx() + b) <= maxPolygon.ry()) && (minPolygon.rx() < end.rx())) {
        QPolygonF points(2);
        points[0] =  QPointF(minPolygon.rx(), (k * minPolygon.rx() + b));
        if((end.rx()<=maxPolygon.rx()) && (end.ry()<=maxPolygon.ry()) && (end.ry()>=minPolygon.ry()))
            points[1] = end;
        else if (((k * maxPolygon.rx() + b) <= maxPolygon.ry()) && ((k * maxPolygon.rx() + b) >= minPolygon.ry()))
            points[1] = QPointF(maxPolygon.rx(), k * maxPolygon.rx() + b);
        else if (((maxPolygon.ry() - b) / k >= minPolygon.rx()) && ((maxPolygon.ry() - b) / k <= maxPolygon.rx()))
            points[1] = QPointF((maxPolygon.ry() - b) / k, maxPolygon.ry());
        else if (((minPolygon.ry() - b) / k >= minPolygon.rx()) && ((minPolygon.ry() - b) / k <= maxPolygon.rx()))
            points[1] = QPointF((minPolygon.ry() - b) / k, minPolygon.ry());
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setTitle("Отсеченный отрезок");
        curve->setPen(Qt::blue, 1);
        coordX.push_back(points[0]);
        coordY.push_back(points[1]);
        curve->setSamples(points);
        curve->attach(ui->widget);
        ui->widget->replot();
        return;
    }
    if ((end.rx() > maxPolygon.rx()) && ((k * maxPolygon.rx() + b) >= minPolygon.ry()) &&
            ((k * maxPolygon.rx() + b) <= maxPolygon.ry()) && (maxPolygon.rx() > begin.rx())) {
        QPolygonF points(2);
        points[1] =  QPointF(maxPolygon.rx(), (k * maxPolygon.rx() + b));
        if (begin.rx() >= minPolygon.rx() && begin.ry() <= maxPolygon.ry() && begin.ry() >= minPolygon.ry())
            points[0] = begin;
        else if (((k * minPolygon.rx() + b) <= maxPolygon.ry()) && ((k * minPolygon.rx() + b) >= minPolygon.ry()))
            points[0] = QPointF(minPolygon.rx(), k * minPolygon.rx() + b);
        else if (((maxPolygon.ry() - b) / k >= minPolygon.rx()) && ((maxPolygon.ry() - b) / k <= maxPolygon.rx()))
            points[0] = QPointF((maxPolygon.ry() - b) / k, maxPolygon.ry());
        else if (((minPolygon.ry() - b) / k >= minPolygon.rx()) && ((minPolygon.ry() - b) / k <= maxPolygon.rx()))
            points[0] = QPointF((minPolygon.ry() - b) / k, minPolygon.ry());
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setTitle("Отсеченный отрезок");
        curve->setPen(Qt::blue, 1);
        coordX.push_back(points[0]);
        coordY.push_back(points[1]);
        curve->setSamples(points);
        curve->attach(ui->widget);
        ui->widget->replot();
        return;
    }
    if ((begin.ry() > maxPolygon.ry()) && ((maxPolygon.ry() - b) / k >= minPolygon.rx()) &&
            ((maxPolygon.ry() - b) / k <= maxPolygon.rx()) && (maxPolygon.ry() > end.ry())) {
        QPolygonF points(2);
        points[0] =  QPointF((maxPolygon.ry() - b) / k, maxPolygon.ry());
        if (end.rx() <= maxPolygon.rx() && end.ry() <= maxPolygon.ry() && end.ry() >= minPolygon.ry())
            points[1] = end;
        else if (((k * maxPolygon.rx() + b) <= maxPolygon.ry()) && ((k * maxPolygon.rx() + b) >= minPolygon.ry()))
            points[1] = QPointF(maxPolygon.rx(), k * maxPolygon.rx() + b);
        else if (((minPolygon.ry() - b) / k >= minPolygon.rx()) && ((minPolygon.ry() - b ) / k <= maxPolygon.rx()))
            points[1] = QPointF((minPolygon.ry() - b) / k, minPolygon.ry());
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setTitle("Отсеченный отрезок");
        curve->setPen(Qt::blue, 1 );
        coordX.push_back(points[0]);
        coordY.push_back(points[1]);
        curve->setSamples(points);
        curve->attach(ui->widget);
        ui->widget->replot();
        return;
    }
    if ((begin.ry() < minPolygon.ry()) && ((minPolygon.ry() - b) / k >= minPolygon.rx()) &&
            ((minPolygon.ry() - b) / k <= maxPolygon.rx()) && (minPolygon.ry() < end.ry())) {
        QPolygonF points(2);
        points[0] = QPointF((minPolygon.ry() - b) / k, minPolygon.ry());
        if (end.rx() <= maxPolygon.rx() && end.ry() <= maxPolygon.ry() && end.ry() >= minPolygon.ry())
            points[1] = end;
        else if (((k * maxPolygon.rx() + b) <= maxPolygon.ry()) && ((k * maxPolygon.rx() + b) >= minPolygon.ry()))
            points[1] = QPointF(maxPolygon.rx(), k * maxPolygon.rx() + b);
        else if (((maxPolygon.ry()-b) / k >= minPolygon.rx()) && ((maxPolygon.ry() - b) / k <= maxPolygon.rx()))
            points[1] = QPointF((maxPolygon.ry() - b) / k, maxPolygon.ry());
        QwtPlotCurve *curve = new QwtPlotCurve();
        curve->setTitle("Отсеченный отрезок");
        curve->setPen(Qt::blue, 1);
        coordX.push_back(points[0]);
        coordY.push_back(points[1]);
        curve->setSamples(points);
        curve->attach(ui->widget);
        ui->widget->replot();
        return;
    }
}

void MainWindow::on_pushButton_clicked()
{
    int size;
    double x1, y1, x2, y2;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream fin(&file);
    fin >> size;
    for (int i = 0; i < size; i++)
    {
        fin >> x1 >> y1 >> x2 >> y2;
        pointBegin.push_back(QPointF(x1, y1));
        pointEnd.push_back(QPointF(x2, y2));
        fill(pointBegin.last(), pointEnd.last());
    }
    fin >> x1 >> y1 >> x2 >> y2;
    minPolygon.setX(x1);
    minPolygon.setY(y1);
    maxPolygon.setX(x2);
    maxPolygon.setY(y2);
    QPolygonF points;
    points << QPointF( x1, y1 ) << QPointF( x1, y2 ) << QPointF( x2, y2 ) << QPointF( x2, y1 ) << QPointF( x1, y1 );
    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle("Многоугольник");
    curve->setPen(Qt::red, 1);
    curve->setSamples(points);
    curve->attach(ui->widget);
    for (int i = 0; i < size; i++)
        midpoint(pointBegin[i], pointEnd[i]);
    ui->widget->replot();
    file.close();
}

void MainWindow::on_pushButton_2_clicked()
{
    int size;
    double x1, y1, x2, y2;
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QTextStream fin(&file);
    fin >> size;
    for (int i = 0; i < size; i++)
    {
        fin >> x1 >> y1 >> x2 >> y2;
        pointBegin.push_back(QPointF(x1,y1));
        pointEnd.push_back(QPointF(x2,y2));
        fill(pointBegin.last(), pointEnd.last());
    }
    fin >> x1 >> y1 >> x2 >> y2;
    minPolygon.setX(x1);
    minPolygon.setY(y1);
    maxPolygon.setX(x2);
    maxPolygon.setY(y2);
    QPolygonF points ;
    points << QPointF( x1, y1 )
            << QPointF( x1, y2 ) << QPointF( x2, y2 )
            << QPointF( x2, y1 ) << QPointF( x1, y1 );
    QwtPlotCurve *curve = new QwtPlotCurve();
    curve->setTitle("Многоугольник");
    curve->setPen(Qt::red, 1);
    curve->setSamples(points);
    curve->attach(ui->widget);
    for (int i = 0; i < size; i++)
        Polygon(pointBegin[i], pointEnd[i]);
    ui->widget->replot();
    file.close();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->QwtPlotDict::detachItems(QwtPlotItem::Rtti_PlotCurve, true);
}

void MainWindow::on_pushButton_4_clicked()
{
    QString arr[] = { ".txt" };
    QString arch;
    QString fileName = QFileDialog ::getOpenFileName(this, tr("Открыть файл"), "D:/", tr("files(*txt)"));
    path = fileName;
}
