#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QMessageBox>
#include <QDebug>
#include <qmath.h>
#include <math.h>
#include <string>

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

void MainWindow::paintEvent(QPaintEvent *e)
{
    QPainter painter(this);
    painter.setPen(colour1);
    painter.setBrush(colour1);
    painter.drawRect(10,200,180,250);
    painter.setPen(colour2);
    painter.setBrush(colour2);
    painter.drawRect(310,200,180,250);
    painter.setPen(colour3);
    painter.setBrush(colour3);
    painter.drawRect(610,200,180,250);
}

void MainWindow::on_pushButton_clicked()
{
    QColor colour = QColorDialog::getColor(colour1, this);
    if(colour.isValid())
    {
        colour.getRgb(&getX, &getY, &getZ);
        button = true;
        on_pushButton_3_clicked();
    }
}

void MainWindow::on_pushButton_2_clicked()
{
    QMessageBox::information(this, "Информация о преобразованиях",
                             "RGB принимает 3 значения от 0 до 255;\n"
                             "HSV принимает 3 значения от 0.0 до 1.0;\n"
                             "HSL принимает 3 значения от 0.0 до 1.0;\n"
                             "XYZ принимает 3 значения от 0.0 до 1.0;\n"
                             "Lab принимает значение L от 0 до 100 и значения A, B от -128 до 128;\n"
                             "CMYK принимает 4 значения от 0 до 255.");
}

//другие преобразования
float Lab_XYZ_function(float x)
{
    if(x>=0.008856)
        return pow(x,1/3.0);
    else
        return 7.787*x+16/116.0;
}

float L_LAB_XYZ(float x,float y,float z)
{
    float l,a,b;
    l=116*Lab_XYZ_function(y)-16;
    a=500*(Lab_XYZ_function(x)-Lab_XYZ_function(y));
    b=200*(Lab_XYZ_function(y)-Lab_XYZ_function(z));
    return l;
}

float A_LAB_XYZ(float x,float y,float z)
{
    float l,a,b;
    l=116*Lab_XYZ_function(y)-16;
    a=500*(Lab_XYZ_function(x)-Lab_XYZ_function(y));
    b=200*(Lab_XYZ_function(y)-Lab_XYZ_function(z));
    return a;
}

float B_LAB_XYZ(double x,double y,double z)
{
    float l,a,b;
    l=116*Lab_XYZ_function(y)-16;
    a=500*(Lab_XYZ_function(x)-Lab_XYZ_function(y));
    b=200*(Lab_XYZ_function(y)-Lab_XYZ_function(z));
    return b;
}

float XYZ_RGB_conv(float x){
    if (x>0.04045)
        return qPow((x+0.055)/1.055, 2.4);
    else
        return x/12.92;
}

float X_XYZ_RGB(float r,float g,float b)
{
    r/=255;
    g/=255;
    b/=255;
    float R = XYZ_RGB_conv(r), G = XYZ_RGB_conv(g), B = XYZ_RGB_conv(b);
    float X, Y, Z;
    X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
    Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
    Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;
    return X;
}

float Y_XYZ_RGB(float r,float g,float b)
{
    r/=255;
    g/=255;
    b/=255;
    float R = XYZ_RGB_conv(r), G = XYZ_RGB_conv(g), B = XYZ_RGB_conv(b);
    float X, Y, Z;
    X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
    Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
    Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;
    return Y;
}

float Z_XYZ_RGB(float r,float g,float b)
{
    r/=255;
    g/=255;
    b/=255;
    float R = XYZ_RGB_conv(r), G = XYZ_RGB_conv(g), B = XYZ_RGB_conv(b);
    float X, Y, Z;
    X = 0.412453 * R + 0.357580 * G + 0.180423 * B;
    Y = 0.212671 * R + 0.715160 * G + 0.072169 * B;
    Z = 0.019334 * R + 0.119193 * G + 0.950227 * B;
    return Z;
}


//RGB в другие модели
QColor CMYK_RGB(int r,int g, int b)
{
    QColor col;
    float c,m,y,k;
    k=std::min(std::min(1.0-r/255.0,1.0-g/255.0),1.0-b/255.0);
    c=(1.0-r/255.0-k)/(1.0-k);
    if(c<0)
        c=0;
    m=(1.0-g/255.0-k)/(1.0-k);
    if(m<0)
        m=0;
    y=(1.0-b/255.0-k)/(1.0-k);
    if(y<0)
        y=0;
    col.setCmykF(c,m,y,k);
    return col;
}

QColor HSV_RGB(float r,float g, float b)
{
    QColor col;
    float h,s,v,CMax,CMin;
    r/=255.0;
    g/=255.0;
    b/=255.0;
    CMax=std::max(std::max(r,g),b);
    CMin=std::min(std::min(r,g),b);
    v=CMax;
    if(v==0)
        s=0;
    else
        s = (CMax-CMin)/CMax;
    if(CMax==CMin)
        h=0;
    else if(CMax==r && g>=b)
        h = 60.0*((g-b)/(CMax-CMin));
    else if(CMax==r && g<b)
        h = 60.0*((g-b)/(CMax-CMin))+360;
    else if(CMax==g)
        h = 60.0*((b-r)/(CMax-CMin))+120;
    else if(CMax==b)
        h = 60.0*((r-g)/(CMax-CMin))+240;
    col.setHsvF(h/360.0,s,v);
    return col;
}

QColor HSL_RGB(float r,float g, float b)
{
    QColor col;
    float h,s,l,CMax,CMin;
    r/=255.0;
    g/=255.0;
    b/=255.0;
    CMax=std::max(std::max(r,g),b);
    CMin=std::min(std::min(r,g),b);
    l=(CMax+CMin)/2.0;
    if(CMax==CMin)
        s=0;
    else
        s = (CMax-CMin)/(1-abs(2*l-1));
    if(CMax==CMin)
        h=0;
    else if(CMax==r && g>=b)
        h = 60.0*((g-b)/(CMax-CMin));
    else if(CMax==r && g<b)
        h = 60.0*((g-b)/(CMax-CMin)) +360;
    else if(CMax==g)
        h = 60.0*((b-r)/(CMax-CMin))+120;
    else if(CMax==b)
        h = 60.0*((r-g)/(CMax-CMin))+240;
    col.setHslF(h/360.0,s,l);
    return col;
}

//CMYK в другие модели
QColor RGB_CMYK(int c,int m, int y, int k)
{
    QColor col;
    int r,g,b;
    r=255*(1-c/255.0)*(1-k/255.0);
    g=255*(1-m/255.0)*(1-k/255.0);
    b=255*(1-y/255.0)*(1-k/255.0);
    col.setRgb(r,g,b);
    return  col;
}

//HSV в другие модели
QColor HSL_HSV(float h,float s, float v)
{
    QColor col;
    float l;
    l=v*(1-s/2);
    if (l==0 || l==1)
        s=0;
    else
        s=(v-l)/std::min(l,1-l);
    col.setHslF(h,s,l);
    return col;
}

QColor RGB_HSV(float h,float s,float v)
{
    QColor col;
    float r,g,b;
    float M=255*v;
    float m=M*(1-s);
    h*=360;
    float mod = h/120;
    mod = mod - (int)mod;
    float  z=(M-m)*(1-abs(2*mod-1));
    r=M;
    g=z+m;
    b=m;
    if(h>=60 && h<120)
    {
        r=z+m;
        g=M;
        b=m;
    }
    else if(h>=120 && h<180)
    {
        r=m;
        g=M;
        b=z+m;
    }
    else if(h>=180 && h<240)
    {
        r=m;
        g=z+m;
        b=M;
    }
    else if(h>=240 && h<300)
    {
        r=z+m;
        g=m;
        b=M;
    }
    else if(h>=300 && h<360)
    {
        r=M;
        g=m;
        b=z+m;
    }
    col.setRgb(r,g,b);
    return col;
}

//HSL в другие модели
QColor RGB_HSL(float h,float s,float l)
{
    QColor col;
    float r,g,b;
    h*=360;
    float mod = h/120;
    mod = mod - (int)mod;
    float c= (1-abs(2*l-1))*s;
    float  x=c*(1-abs(2*mod-1));
    float m = l - c/2;
    if (h>=0 && h<60)
    {
        r=c;
        g=x;
        b=0;
    }
    else if(h>=60 && h<120)
    {
        r=x;
        g=c;
        b=0;
    }
    else if(h>=120 && h<=180)
    {
        r=0;
        g=c;
        b=x;
    }
    else if(h>=180 && h<=240)
    {
        r=0;
        g=x;
        b=c;
    }
    else if(h>=240 && h<=300)
    {
        r=x;
        g=0;
        b=c;
    }
    else if(h>=300 && h<=360)
    {
        r=c;
        g=0;
        b=x;
    }
    col.setRgb(255*(r+m),255*(g+m),255*(b+m));
    return col;
}

QColor HSV_HSL(float h,float s, float l)
{
    QColor col;
    float v;
    v=l+s*std::min(l,1-l);
    if(v==0)
        s=0;
    else
        s=2*(1-l/v);
    col.setHsvF(h,s,v);
    return  col;
}

//XYZ в другие модели
float convert(float num)
{
    if(num >=0.0031308)
        return 1.055*pow(num,1/2.4) - 0.055;
    else
        return 12.96*num;
}

QColor RGB_XYZ(float x,float y, float z)
{
    QColor col;
    float r,g,b;
    r =  3.2406*x - 1.5372*y - 0.4986*z;
    g = -0.9689*x + 1.8758*y + 0.0415*z;
    b =  0.0557*x - 0.2040*y + 1.0570*z;
    col.setRgbF(convert(r),convert(g),convert(b));
    return col;
}

//LAB в другие модели
float XYZ_Lab_function(double x)
{
    if(pow(x,3)>=0.008856)
        return pow(x,3);
    else
        return (x-16/116.0)/7.787;
}

QColor RGB_LAB(float l,float a,float b){
    QColor col;
    float x,y,z;
    y=XYZ_Lab_function((l+16)/116.0)*95.047;
    x=XYZ_Lab_function(a/500.0+(l+16)/116.0)*100;
    z=XYZ_Lab_function((l+16)/116.0-b/200.0)*108.883;
    col= RGB_XYZ(x/100.0,y/100.0,z/100.0);
    return col;
}

//из RGB
void MainWindow::on_pushButton_3_clicked()
{
    int x,y,z;
    QString str1;
    if (!button)
    {
        str1 = ui->lineEdit->text();
        x = str1.toInt();
        str1 = ui->lineEdit_2->text();
        y = str1.toInt();
        str1 = ui->lineEdit_3->text();
        z = str1.toInt();
    }
    else
    {
        x=getX;
        y=getY;
        z=getZ;
        button = false;
        ui->lineEdit->setText(str1.setNum(x));
        ui->lineEdit_2->setText(str1.setNum(y));
        ui->lineEdit_3->setText(str1.setNum(z));
    }
    if (x > 255 || y > 255 || z > 255 || x<0 || y<0 || z<0)
    {
        colour1.setRgb(255,255,255);
        ui->label_26->setText("Вне диапазона!");
        colour2.setRgb(255,255,255);
        ui->label_27->setText("Вне диапазона!");
        colour3.setRgb(255,255,255);
        ui->label_28->setText("Вне диапазона!");
    }
    else
    {
         QColor tmp;
         QString filler;
         tmp = HSV_RGB(x,y,z);
         ui->lineEdit_9->setText(filler.setNum(tmp.hueF()));
         ui->lineEdit_4->setText(filler.setNum(tmp.saturationF()));
         ui->lineEdit_10->setText(filler.setNum(tmp.valueF()));
         tmp = HSL_RGB(x,y,z);
         qreal t,u,b;
         tmp.getHslF(&t,&u,&b);
         ui->lineEdit_11->setText(filler.setNum(t));
         ui->lineEdit_12->setText(filler.setNum(u));
         ui->lineEdit_13->setText(filler.setNum(b));

         ui->lineEdit_14->setText(filler.setNum(X_XYZ_RGB(x,y,z)));
         ui->lineEdit_15->setText(filler.setNum(Y_XYZ_RGB(x,y,z)));
         ui->lineEdit_16->setText(filler.setNum(Z_XYZ_RGB(x,y,z)));

         float o=X_XYZ_RGB(x,y,z), p=Y_XYZ_RGB(x,y,z), i=Z_XYZ_RGB(x,y,z);
         ui->lineEdit_17->setText(filler.setNum(L_LAB_XYZ(o,p,i)));
         ui->lineEdit_18->setText(filler.setNum(A_LAB_XYZ(o,p,i)));
         ui->lineEdit_19->setText(filler.setNum(B_LAB_XYZ(o,p,i)));

         tmp = CMYK_RGB(x,y,z);
         ui->lineEdit_20->setText(filler.setNum(tmp.cyan()));
         ui->lineEdit_21->setText(filler.setNum(tmp.magenta()));
         ui->lineEdit_22->setText(filler.setNum(tmp.yellow()));
         ui->lineEdit_23->setText(filler.setNum(tmp.black()));
         str1 = (ui->comboBox->currentText());
         if (str1=="RGB")
         {
             colour1.setRgb(x,y,z);
             ui->label_26->setText("");
         }
         if (str1=="CMYK")
         {
             colour1 = CMYK_RGB(x,y,z);
             ui->label_26->setText("");
         }
         if (str1=="HSV")
         {
             colour1 = HSV_RGB(x,y,z);
             ui->label_26->setText("");
         }
         if (str1=="HSL")
         {
             colour1 = HSL_RGB(x,y,z);
             ui->label_26->setText("");
         }
         QString str_2 = (ui->comboBox_2->currentText());
         if (str_2=="RGB")
         {
             colour2.setRgb(x,y,z);
             ui->label_27->setText("");
         }
         if (str_2=="CMYK")
         {
             colour2 = CMYK_RGB(x,y,z);
             ui->label_27->setText("");
         }
         if (str_2=="HSV")
         {
             colour2 = HSV_RGB(x,y,z);
             ui->label_27->setText("");
         }
         if (str_2=="HSL")
         {
             colour2 = HSL_RGB(x,y,z);
             ui->label_27->setText("");
         }
         QString str_3 = (ui->comboBox_3->currentText());
         if (str_3=="RGB")
         {
             colour3.setRgb(x,y,z);
             ui->label_28->setText("");
         }
         if (str_3=="CMYK")
         {
             colour3 = CMYK_RGB(x,y,z);
             ui->label_28->setText("");
         }
         if (str_3=="HSV")
         {
             colour3 = HSV_RGB(x,y,z);
             ui->label_28->setText("");
         }
         if (str_3=="HSL")
         {
             colour3 = HSL_RGB(x,y,z);
             ui->label_28->setText("");
         }
    }
    update();
}

//из HSV
void MainWindow::on_pushButton_4_clicked()
{
    float h,s,v;
    QString str = ui->lineEdit_9->text();
    h = str.toFloat();
    str = ui->lineEdit_4->text();
    s = str.toFloat();
    str = ui->lineEdit_10->text();
    v = str.toFloat();
    if (h > 1 || s > 1 || v > 1 || h<0 || s<0 || v<0)
    {
        colour1.setRgb(255,255,255);
        ui->label_26->setText("Вне диапазона!");
        colour2.setRgb(255,255,255);
        ui->label_27->setText("Вне диапазона!");
        colour3.setRgb(255,255,255);
        ui->label_28->setText("Вне диапазона!");
    }
    else
    {
        QColor tmp=RGB_HSV(h,s,v), converted;
        QString filler;
        ui->lineEdit->setText(filler.setNum(tmp.red()));
        ui->lineEdit_2->setText(filler.setNum(tmp.green()));
        ui->lineEdit_3->setText(filler.setNum(tmp.blue()));
        converted = HSL_RGB(tmp.red(),tmp.green(),tmp.blue());
        qreal t,u,b;
        converted.getHslF(&t,&u,&b);
        ui->lineEdit_11->setText(filler.setNum(t));
        ui->lineEdit_12->setText(filler.setNum(u));
        ui->lineEdit_13->setText(filler.setNum(b));

        ui->lineEdit_14->setText(filler.setNum(X_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));
        ui->lineEdit_15->setText(filler.setNum(Y_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));
        ui->lineEdit_16->setText(filler.setNum(Z_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));

        float o=X_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        float p=Y_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue()),i=Z_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_17->setText(filler.setNum(L_LAB_XYZ(o,p,i)));
        ui->lineEdit_18->setText(filler.setNum(A_LAB_XYZ(o,p,i)));
        ui->lineEdit_19->setText(filler.setNum(B_LAB_XYZ(o,p,i)));

        converted = CMYK_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_20->setText(filler.setNum(converted.cyan()));
        ui->lineEdit_21->setText(filler.setNum(converted.magenta()));
        ui->lineEdit_22->setText(filler.setNum(converted.yellow()));
        ui->lineEdit_23->setText(filler.setNum(converted.black()));
        str = (ui->comboBox->currentText());
        if (str=="RGB")
        {
            colour1 = RGB_HSV(h,s,v);
            ui->label_26->setText("");
        }
        if (str=="CMYK")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        if (str=="HSV")
        {
            colour1.setHsvF(h,s,v);
            ui->label_26->setText("");
        }
        if (str=="HSL")
        {
            colour1 = HSL_HSV(h,s,v);
            ui->label_26->setText("");
        }
        QString str_2 = (ui->comboBox_2->currentText());
        if (str_2=="RGB")
        {
            colour2 = RGB_HSV(h,s,v);
            ui->label_27->setText("");
        }
        if (str_2=="CMYK")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        if (str_2=="HSV")
        {
            colour2.setHsvF(h,s,v);
            ui->label_27->setText("");
        }
        if (str_2=="HSL")
        {
            colour2 = HSL_HSV(h,s,v);
            ui->label_27->setText("");
        }
        QString str_3 = (ui->comboBox_3->currentText());
        if (str_3=="RGB")
        {
            colour3 = RGB_HSV(h,s,v);
            ui->label_28->setText("");
        }
        if (str_3=="CMYK")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
        if (str_3=="HSV")
        {
            colour3.setHsvF(h,s,v);
            ui->label_28->setText("");
        }
        if (str_3=="HSL")
        {
            colour3 = HSL_HSV(h,s,v);
            ui->label_28->setText("");
        }
    }
    update();
}

//из HSL
void MainWindow::on_pushButton_5_clicked()
{
    float h,s,l;
    QString str = ui->lineEdit_11->text();
    h = str.toFloat();
    str = ui->lineEdit_12->text();
    s = str.toFloat();
    str = ui->lineEdit_13->text();
    l = str.toFloat();
    if (h > 1 || s > 1 || l > 1 || h<0 || s<0 || l<0)
    {
        colour1.setRgb(255,255,255);
        ui->label_26->setText("Вне диапазона!");
        colour2.setRgb(255,255,255);
        ui->label_27->setText("Вне диапазона!");
        colour3.setRgb(255,255,255);
        ui->label_28->setText("Вне диапазона!");
    }
    else
    {
        QColor tmp=RGB_HSL(h,s,l),converted;
        QString filler;
        ui->lineEdit->setText(filler.setNum(tmp.red()));
        ui->lineEdit_2->setText(filler.setNum(tmp.green()));
        ui->lineEdit_3->setText(filler.setNum(tmp.blue()));
        converted = HSV_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_9->setText(filler.setNum(converted.hueF()));
        ui->lineEdit_4->setText(filler.setNum(converted.saturationF()));
        ui->lineEdit_10->setText(filler.setNum(converted.valueF()));

        ui->lineEdit_14->setText(filler.setNum(X_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));
        ui->lineEdit_15->setText(filler.setNum(Y_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));
        ui->lineEdit_16->setText(filler.setNum(Z_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));

        float o=X_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        float p=Y_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue()),i=Z_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_17->setText(filler.setNum(L_LAB_XYZ(o,p,i)));
        ui->lineEdit_18->setText(filler.setNum(A_LAB_XYZ(o,p,i)));
        ui->lineEdit_19->setText(filler.setNum(B_LAB_XYZ(o,p,i)));

        converted = CMYK_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_20->setText(filler.setNum(converted.cyan()));
        ui->lineEdit_21->setText(filler.setNum(converted.magenta()));
        ui->lineEdit_22->setText(filler.setNum(converted.yellow()));
        ui->lineEdit_23->setText(filler.setNum(converted.black()));
        str = (ui->comboBox->currentText());
        if (str=="RGB")
        {
            colour1 = RGB_HSL(h,s,l);
            ui->label_26->setText("");
        }
        if (str=="CMYK")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        if (str=="HSV")
        {
            colour1 = HSV_HSL(h,s,l);
            ui->label_26->setText("");
        }
        if (str=="HSL")
        {
            colour1.setHslF(h,s,l);
            ui->label_26->setText("");
        }
        QString str_2 = (ui->comboBox_2->currentText());
        if (str_2=="RGB")
        {
            colour2 = RGB_HSL(h,s,l);
            ui->label_27->setText("");
        }
        if (str_2=="CMYK")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        if (str_2=="HSV")
        {
            colour2 = HSV_HSL(h,s,l);
            ui->label_27->setText("");
        }
        if (str_2=="HSL")
        {
            colour2.setHslF(h,s,l);
            ui->label_27->setText("");
        }
        QString str_3 = (ui->comboBox_3->currentText());
        if (str_3=="RGB")
        {
            colour3 = RGB_HSL(h,s,l);
            ui->label_28->setText("");
        }
        if (str_3=="CMYK")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
        if (str_3=="HSV")
        {
            colour3 = HSV_HSL(h,s,l);
            ui->label_28->setText("");
        }
        if (str_3=="HSL")
        {
            colour3.setHslF(h,s,l);
            ui->label_28->setText("");
        }
    }
    update();
}

//из XYZ
void MainWindow::on_pushButton_6_clicked()
{
    float x,y,z;
    QString str = ui->lineEdit_14->text();
    x = str.toFloat();
    str = ui->lineEdit_15->text();
    y = str.toFloat();
    str = ui->lineEdit_16->text();
    z = str.toFloat();
    if (x > 1 || y > 1 || z > 1 || x<0 || y<0 || z<0)
    {
        colour1.setRgb(255,255,255);
        ui->label_26->setText("Вне диапазона!");
        colour2.setRgb(255,255,255);
        ui->label_27->setText("Вне диапазона!");
        colour3.setRgb(255,255,255);
        ui->label_28->setText("Вне диапазона!");
    }
    else
    {
        QColor tmp=RGB_XYZ(x,y,z),converted;
        QString filler;
        ui->lineEdit->setText(filler.setNum(tmp.red()));
        ui->lineEdit_2->setText(filler.setNum(tmp.green()));
        ui->lineEdit_3->setText(filler.setNum(tmp.blue()));
        converted = HSL_RGB(tmp.red(),tmp.green(),tmp.blue());
        qreal t,u,b;
        converted.getHslF(&t,&u,&b);
        ui->lineEdit_11->setText(filler.setNum(t));
        ui->lineEdit_12->setText(filler.setNum(u));
        ui->lineEdit_13->setText(filler.setNum(b));
        converted = HSV_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_9->setText(filler.setNum(converted.hueF()));
        ui->lineEdit_4->setText(filler.setNum(converted.saturationF()));
        ui->lineEdit_10->setText(filler.setNum(converted.valueF()));

        float o=X_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        float p=Y_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue()),i=Z_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_17->setText(filler.setNum(L_LAB_XYZ(o,p,i)));
        ui->lineEdit_18->setText(filler.setNum(A_LAB_XYZ(o,p,i)));
        ui->lineEdit_19->setText(filler.setNum(B_LAB_XYZ(o,p,i)));

        converted = CMYK_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_20->setText(filler.setNum(converted.cyan()));
        ui->lineEdit_21->setText(filler.setNum(converted.magenta()));
        ui->lineEdit_22->setText(filler.setNum(converted.yellow()));
        ui->lineEdit_23->setText(filler.setNum(converted.black()));
        str = (ui->comboBox->currentText());
        if (str=="RGB")
        {
            colour1 = RGB_XYZ(x,y,z);
            ui->label_26->setText("");
        }
        if (str=="CMYK")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        if (str=="HSV")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        if (str=="HSL")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        QString str_2 = (ui->comboBox_2->currentText());
        if (str_2=="RGB")
        {
            colour2 = RGB_XYZ(x,y,z);
            ui->label_27->setText("");
        }
        if (str_2=="CMYK")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        if (str_2=="HSV")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        if (str_2=="HSL")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        QString str_3 = (ui->comboBox_3->currentText());
        if (str_3=="RGB")
        {
            colour3 = RGB_XYZ(x,y,z);
            ui->label_28->setText("");
        }
        if (str_3=="CMYK")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
        if (str_3=="HSV")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
        if (str_3=="HSL")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
    }
    update();
}

//из Lab
void MainWindow::on_pushButton_7_clicked()
{
    float l,a,b;
    QString str = ui->lineEdit_17->text();
    l = str.toFloat();
    str = ui->lineEdit_18->text();
    a = str.toFloat();
    str = ui->lineEdit_19->text();
    b = str.toFloat();
    if (l < 0 || a < -128 || b < -128 || l>100 || a>128 || b>128)
    {
        colour1.setRgb(255,255,255);
        ui->label_26->setText("Вне диапазона!");
        colour2.setRgb(255,255,255);
        ui->label_27->setText("Вне диапазона!");
        colour3.setRgb(255,255,255);
        ui->label_28->setText("Вне диапазона!");
    }
    else
    {
        QColor tmp=RGB_LAB(l,a,b),converted;
        QString filler;
        ui->lineEdit->setText(filler.setNum(tmp.red()));
        ui->lineEdit_2->setText(filler.setNum(tmp.green()));
        ui->lineEdit_3->setText(filler.setNum(tmp.blue()));
        converted = HSL_RGB(tmp.red(),tmp.green(),tmp.blue());
        qreal t,u,b;
        converted.getHslF(&t,&u,&b);
        ui->lineEdit_11->setText(filler.setNum(t));
        ui->lineEdit_12->setText(filler.setNum(u));
        ui->lineEdit_13->setText(filler.setNum(b));
        converted = HSV_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_9->setText(filler.setNum(converted.hueF()));
        ui->lineEdit_4->setText(filler.setNum(converted.saturationF()));
        ui->lineEdit_10->setText(filler.setNum(converted.valueF()));

        ui->lineEdit_14->setText(filler.setNum(X_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));
        ui->lineEdit_15->setText(filler.setNum(Y_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));
        ui->lineEdit_16->setText(filler.setNum(Z_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue())));
        converted = CMYK_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_20->setText(filler.setNum(converted.cyan()));
        ui->lineEdit_21->setText(filler.setNum(converted.magenta()));
        ui->lineEdit_22->setText(filler.setNum(converted.yellow()));
        ui->lineEdit_23->setText(filler.setNum(converted.black()));
        str = (ui->comboBox->currentText());
        if (str=="RGB")
        {
            colour1 = RGB_LAB(l,a,b);
            ui->label_26->setText("");
        }
        if (str=="CMYK")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        if (str=="HSV")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        if (str=="HSL")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        QString str_2 = (ui->comboBox_2->currentText());
        if (str_2=="RGB")
        {
            colour2 = RGB_LAB(l,a,b);
            ui->label_27->setText("");
        }
        if (str_2=="CMYK")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        if (str_2=="HSV")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        if (str_2=="HSL")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        QString str_3 = (ui->comboBox_3->currentText());
        if (str_3=="RGB")
        {
            colour3 = RGB_LAB(l,a,b);
            ui->label_28->setText("");
        }
        if (str_3=="CMYK")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
        if (str_3=="HSV")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
        if (str_3=="HSL")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
    }
    update();
}

//из CMYK
void MainWindow::on_pushButton_8_clicked()
{
    int c,m,y,k;
    QString str = ui->lineEdit_20->text();
    c = str.toInt();
    str = ui->lineEdit_21->text();
    m = str.toInt();
    str = ui->lineEdit_22->text();
    y = str.toInt();
    str = ui->lineEdit_23->text();
    k = str.toInt();
    if (c > 255 || m > 255 || y > 255 || k > 255|| c<0 || m<0 || y<0 || k<0)
    {
        colour1.setRgb(255,255,255);
        ui->label_26->setText("Вне диапазона!");
        colour2.setRgb(255,255,255);
        ui->label_27->setText("Вне диапазона!");
        colour3.setRgb(255,255,255);
        ui->label_28->setText("Вне диапазона!");
    }
    else
    {
        QColor tmp=RGB_CMYK(c,m,y,k),converted;
        QString filler;
        ui->lineEdit->setText(filler.setNum(tmp.red()));
        ui->lineEdit_2->setText(filler.setNum(tmp.green()));
        ui->lineEdit_3->setText(filler.setNum(tmp.blue()));
        converted = HSL_RGB(tmp.red(),tmp.green(),tmp.blue());
        qreal t,u,b;
        converted.getHslF(&t,&u,&b);
        ui->lineEdit_11->setText(filler.setNum(t));
        ui->lineEdit_12->setText(filler.setNum(u));
        ui->lineEdit_13->setText(filler.setNum(b));
        converted = HSV_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_9->setText(filler.setNum(converted.hueF()));
        ui->lineEdit_4->setText(filler.setNum(converted.saturationF()));
        ui->lineEdit_10->setText(filler.setNum(converted.valueF()));
        converted = HSV_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_14->setText(filler.setNum(converted.hueF()));
        ui->lineEdit_15->setText(filler.setNum(converted.saturationF()));
        ui->lineEdit_16->setText(filler.setNum(converted.valueF()));

        float o=X_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        float p=Y_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue()),i=Z_XYZ_RGB(tmp.red(),tmp.green(),tmp.blue());
        ui->lineEdit_17->setText(filler.setNum(L_LAB_XYZ(o,p,i)));
        ui->lineEdit_18->setText(filler.setNum(A_LAB_XYZ(o,p,i)));
        ui->lineEdit_19->setText(filler.setNum(B_LAB_XYZ(o,p,i)));
        str = (ui->comboBox->currentText());
        if (str=="RGB")
        {
            colour1 = RGB_CMYK(c,m,y,k);
            ui->label_26->setText("");
        }
        if (str=="CMYK")
        {
            colour1.setCmyk(c,m,y,k);
            ui->label_26->setText("");
        }
        if (str=="HSV")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        if (str=="HSL")
        {
            colour1.setRgb(255,255,255);
            ui->label_26->setText("Неправильное преобразование!");
        }
        QString str_2 = (ui->comboBox_2->currentText());
        if (str_2=="RGB")
        {
            colour2 = RGB_CMYK(c,m,y,k);
            ui->label_27->setText("");
        }
        if (str_2=="CMYK")
        {
            colour2.setCmyk(c,m,y,k);
            ui->label_27->setText("");
        }
        if (str_2=="HSV")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        if (str_2=="HSL")
        {
            colour2.setRgb(255,255,255);
            ui->label_27->setText("Неправильное преобразование!");
        }
        QString str_3 = (ui->comboBox_3->currentText());
        if (str_3=="RGB")
        {
            colour3 = RGB_CMYK(c,m,y,k);
            ui->label_28->setText("");
        }
        if (str_3=="CMYK")
        {
            colour3.setCmyk(c,m,y,k);
            ui->label_28->setText("");
        }
        if (str_3=="HSV")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
        if (str_3=="HSL")
        {
            colour3.setRgb(255,255,255);
            ui->label_28->setText("Неправильное преобразование!");
        }
    }
    update();
}

void MainWindow::on_pushButton_9_clicked()
{
    QString filler;
    ui->lineEdit->setText(filler.setNum(0));
    ui->lineEdit_2->setText(filler.setNum(0));
    ui->lineEdit_3->setText(filler.setNum(0));
    ui->lineEdit_9->setText(filler.setNum(0));
    ui->lineEdit_4->setText(filler.setNum(0));
    ui->lineEdit_10->setText(filler.setNum(0));
    ui->lineEdit_11->setText(filler.setNum(0));
    ui->lineEdit_12->setText(filler.setNum(0));
    ui->lineEdit_13->setText(filler.setNum(0));
    ui->lineEdit_14->setText(filler.setNum(0));
    ui->lineEdit_15->setText(filler.setNum(0));
    ui->lineEdit_16->setText(filler.setNum(0));
    ui->lineEdit_17->setText(filler.setNum(0));
    ui->lineEdit_18->setText(filler.setNum(0));
    ui->lineEdit_19->setText(filler.setNum(0));
    ui->lineEdit_20->setText(filler.setNum(0));
    ui->lineEdit_21->setText(filler.setNum(0));
    ui->lineEdit_22->setText(filler.setNum(0));
    ui->lineEdit_23->setText(filler.setNum(0));
}
