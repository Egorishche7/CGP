#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "letter.h"
#include <QMessageBox>

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

void MainWindow::on_pushButton_clicked()
{
    ui->widget->check = 1;
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->widget->check = 2;
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_3_clicked()
{
    ui->widget->check = 3;
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_4_clicked()
{
    QString line;
    line = ui->lineEdit->text();
    ui->widget->xScale = line.toFloat();
    line = ui->lineEdit_2->text();
    ui->widget->yScale=line.toFloat();
    line = ui->lineEdit_3->text();
    ui->widget->zScale = line.toFloat();
    ui->widget->check = 4;
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString line;
    line = ui->lineEdit_4->text();
    ui->widget->xTransfer = line.toFloat();
    line = ui->lineEdit_5->text();
    ui->widget->yTransfer = line.toFloat();
    line = ui->lineEdit_6->text();
    ui->widget->zTransfer = line.toFloat();
    ui->widget->check = 5;
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_6_clicked()
{
    QString line;
    line = ui->lineEdit_7->text();
    ui->widget->check = 6;
    ui->widget->angle = line.toFloat();
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_7_clicked()
{
    QString line;
    line = ui->lineEdit_8->text();
    ui->widget->check = 7;
    ui->widget->angle = line.toFloat();
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_8_clicked()
{
    QString line;
    line = ui->lineEdit_9->text();
    ui->widget->check = 8;
    ui->widget->angle = line.toFloat();
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_9_clicked()
{
    ui->widget->check = 9;
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_10_clicked()
{
    ui->widget->colour = QColorDialog::getColor(Qt::black, this);
}

void MainWindow::keyPressEvent(QKeyEvent* e)
{
    ui->widget->keyPressEvent(e);
    ui->widget->updateGL();
}

void MainWindow::on_pushButton_11_clicked()
{
    QMessageBox::information(this, "Управление клавишами",
                             "Для управления видом можно использовать мышь или клавиатуру:\n"
                             "Увеличить мастшаб +\n"
                             "Уменьшить мастшаб -\n"
                             "Повернуть вверх w\n"
                             "Повернуть вниз s\n"
                             "Повернуть влево a\n"
                             "Повернуть вправо d\n"
                             "Сдвинуть вверх z\n"
                             "Сдвинуть вниз x\n");
}
