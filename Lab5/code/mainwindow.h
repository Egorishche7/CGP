#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <qwt_plot_curve.h>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void fill(QPointF begin, QPointF end);
    void midpoint(QPointF begin, QPointF end);
    void Polygon(QPointF begin, QPointF end);
    QVector<QwtPlotCurve*> v;
    QVector<QPointF> pointBegin;
    QVector<QPointF> pointEnd;
    QVector<QPointF> coordX;
    QVector<QPointF> coordY;
    QPointF minPolygon, maxPolygon;
    QString path;
private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
