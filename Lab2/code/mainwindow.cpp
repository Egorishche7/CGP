#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <qzipreader_p.h>
#include <qzipwriter_p.h>
#include <QFileDialog>
#include <QMessageBox>
#include <QFileInfo>
#include <QDebug>
#include <QImage>
#include <QtCore>
#include <QImageWriter>
#include <QStyledItemDelegate>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QString arr[] = { ".jpg", ".gif", ".tif", ".bmp"," .png", ".pcx" };
    QString arch;
    bool check = false;
    QStringList fileName = QFileDialog ::getOpenFileNames(this,
                                                          tr("Открыть файл"),
                                                          "D:/",
                                                          tr("Images(*jpg *gif *tif *bmp *png *pcx);;archive(*zip)"));
    QTime t1 = QTime::currentTime();
    if (fileName[0].contains("zip"))
    {
        QFileInfo tmp(fileName[0]);
        arch = tmp.path();
        QZipReader zip_reader(fileName[0]);
        if (zip_reader.exists())
        {
            foreach (QZipReader::FileInfo info, zip_reader.fileInfoList())
            {
               for (int i = 0; i < arr->size(); i++)
                   if (info.filePath.contains(arr[i]))
                   {
                       check = true;
                       break;
                   }
               if (check)
               {
                   break;
               }
            }
            qDebug() << arch;
            arch = arch + "/myFolder";
            QDir dir(arch);
            if (!dir.exists())
                dir.mkpath(arch);
            if (check)
            {
                zip_reader.extractAll(arch);
                QStringList filters;
                filters << "*.jpg" << "*.gif" << "*.tif" << "*.bmp" << "*.png" << "*.pcx";
                dir.setNameFilters(filters);
                QFileInfoList jpgs = dir.entryInfoList();
                for (int i = 0; i < jpgs.size(); i++)
                {
                    QFileInfo fileinfo = jpgs[i];
                    QString path = fileinfo.path();
                    QString str = fileinfo.fileName();
                    QImageWriter inf;
                    inf.setFileName(fileinfo.fileName());
                    QImage myImage;
                    path = path + "/" + str;
                    myImage.load(path);
                    int size = fileinfo.size();
                    QString res = QVariant(myImage.width()).toString() + "x" + QVariant(myImage.height()).toString();
                    ui->tableWidget->setColumnCount(6);
                    ui->tableWidget->setRowCount(count + 1);
                    ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Имя файла"));
                    ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Расширение"));
                    ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Размер изображения"));
                    ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Разрешение"));
                    ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Глубина цвета"));
                    ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Сжатие"));
                    ui->tableWidget->setItem(count, 0, new QTableWidgetItem(str));
                    ui->tableWidget->setItem(count, 1, new QTableWidgetItem(fileinfo.suffix()));
                    ui->tableWidget->setItem(count, 2, new QTableWidgetItem(QVariant(size).toString()));
                    ui->tableWidget->setItem(count, 3, new QTableWidgetItem(res));
                    ui->tableWidget->setItem(count, 4, new QTableWidgetItem(QVariant(myImage.depth()).toString()));
                    ui->tableWidget->setItem(count, 5, new QTableWidgetItem(QVariant(inf.compression()).toString()));
                    count++;
                }
                ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
                ui->tableWidget->setSortingEnabled(true);
                for (int i = 0; i < ui->tableWidget->rowCount(); i++)
                {
                    QTableWidgetItem* it = new QTableWidgetItem();
                    int number;
                    QTableWidgetItem *item = ui->tableWidget->item(i,2);
                    if (item != nullptr)
                       number = (item->text()).toInt();
                    it->setData(Qt::DisplayRole, number);
                    ui->tableWidget->setItem(i, 2, it);
                }
            }
            dir.removeRecursively();
        }
    }
    else
    {
        for (int i = 0; i < fileName.size(); i++)
        {
            QFileInfo fileinfo(fileName[i]);
            QString path = fileinfo.path();
            QString str = fileinfo.fileName();
            QImageWriter inf;
            inf.setFileName(fileinfo.fileName());
            QImage myImage;
            path = path + "/" + str;
            myImage.load(path);
            int size = fileinfo.size();
            QString res = QVariant(myImage.width()).toString() + "x" + QVariant(myImage.height()).toString();
            ui->tableWidget->setColumnCount(6);
            ui->tableWidget->setRowCount(count + 1);
            ui->tableWidget->setHorizontalHeaderItem(0, new QTableWidgetItem("Имя файла"));
            ui->tableWidget->setHorizontalHeaderItem(1, new QTableWidgetItem("Расширение"));
            ui->tableWidget->setHorizontalHeaderItem(2, new QTableWidgetItem("Размер изображения"));
            ui->tableWidget->setHorizontalHeaderItem(3, new QTableWidgetItem("Разрешение"));
            ui->tableWidget->setHorizontalHeaderItem(4, new QTableWidgetItem("Глубина цвета"));
            ui->tableWidget->setHorizontalHeaderItem(5, new QTableWidgetItem("Сжатие"));
            ui->tableWidget->setItem(count, 0, new QTableWidgetItem(str));
            ui->tableWidget->setItem(count, 1, new QTableWidgetItem(fileinfo.suffix()));
            ui->tableWidget->setItem(count, 2, new QTableWidgetItem(QVariant(size).toString()));
            ui->tableWidget->setItem(count, 3, new QTableWidgetItem(res));
            ui->tableWidget->setItem(count, 4, new QTableWidgetItem(QVariant(myImage.depth()).toString()));
            ui->tableWidget->setItem(count, 5, new QTableWidgetItem(QVariant(inf.compression()).toString()));
            count++;
        }
        ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);
        ui->tableWidget->setSortingEnabled(true);
        for (int i = 0; i < ui->tableWidget->rowCount(); i++)
        {
            QTableWidgetItem* it = new QTableWidgetItem();
            int number;
            QTableWidgetItem *item = ui->tableWidget->item(i, 2);
            if (item != nullptr)
               number = (item->text()).toInt();
            it->setData(Qt::DisplayRole, number);
            ui->tableWidget->setItem(i, 2, it);
        }
    }
    quint64 totalMS = qAbs(QTime::currentTime().msecsTo(t1));
    QTime t2 = fromMS(totalMS);
    qDebug() << "Для считывания информации потребовалось времени:" << t2;
}
