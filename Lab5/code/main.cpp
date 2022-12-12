#include "mainwindow.h"
#include <QApplication>
#include <qwt_plot.h>

QPointF minPolygon(5, 5), maxPolygon(15, 15);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
