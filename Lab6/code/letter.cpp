#include "letter.h"
#include "ui_mainwindow.h"
#include <QDebug>

GLfloat vertexArr[36][3];
GLubyte idxArr[9][4];

letter::letter(QWidget *parent)
    : QGLWidget(parent)
{
    setGeometry(400, 200, 800, 600);
    xRotate = yRotate = zRotate = 0;
    zTranslation = -1;
    scale = 1;
}

void letter::initializeGL()
{
    qglClearColor(Qt::white);
    glEnable(GL_DEPTH_TEST);
    glShadeModel(GL_FLAT);
    glEnableClientState(GL_VERTEX_ARRAY);
}

void letter::resizeGL(int width, int height)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat ratio = (GLfloat)height / (GLfloat)width;
    if (width >= height)
       glOrtho(-1.0 / ratio, 1.0 / ratio, -1.0, 1.0, -5.0, 5.0);
    else
       glOrtho(-1.0, 1.0, -1.0 * ratio, 1.0 * ratio, -5.0, 5.0);
    glViewport(0, 0, (GLint)width, (GLint)height);
}

void letter::paintGL()
{
    glClearColor(1,1,1,0);
    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glScalef(scale, scale, scale);
    glTranslatef(0.0f, zTranslation, 0.0f);
    glRotatef(xRotate, 1.0f, 0.0f, 0.0f);
    glRotatef(yRotate, 0.0f, 1.0f, 0.0f);
    glRotatef(zRotate, 0.0f, 0.0f, 1.0f);
    drawAxis();
    double e = 0, r = 0, t = 0;
    colour.getRgbF(&e, &r, &t, nullptr);
    glColor4f(e, r, t, 0.0f);
    getVertexArray(1);
    getIndexArray();
    if (check == 1)
        projectionXY();
    else if (check == 2)
        projectionXZ();
    else if (check == 3)
        projectionYZ();
    else if (check == 4)
        scaling();
    else if (check == 5)
        translation();
    else if (check == 6)
        rotationX();
    else if (check == 7)
        rotationY();
    else if (check == 8)
        rotationZ();
    else if (check == 9)
        drawFigure();
    QFont font;
    font.setFamily("Arial Black");
    font.setPointSize(10);
    font.setBold(false);
    glColor3f(0, 0, 0);
    renderText(10, 0.0, 0.0, "X", font);
    glColor3f(0, 0, 0);
    renderText(0.0, 10, 0.0, "Y", font);
    glColor3f(0, 0, 0);
    renderText(0.0, 0.0, 10, "Z", font);
}

void letter::mousePressEvent(QMouseEvent* e)
{
    ptrMousePos = e->pos();
}

void letter::mouseMoveEvent(QMouseEvent* e)
{
    xRotate += 1 / M_PI * (GLfloat)(e->y()-ptrMousePos.y());
    yRotate += 1 / M_PI * (GLfloat)(e->x()-ptrMousePos.x());
    ptrMousePos = e->pos();
    updateGL();
}

void letter::wheelEvent(QWheelEvent* e)
{
   if ((e->delta()) > 0)
       scalePlus();
   else if ((e->delta()) < 0)
       scaleMinus();
   updateGL();
}

void letter::keyPressEvent(QKeyEvent* e)
{
    switch (e->key())
    {
       case Qt::Key_Plus:
        scalePlus();
        break;
       case Qt::Key_Equal:
        scalePlus();
        break;
       case Qt::Key_Minus:
        scaleMinus();
        break;
       case Qt::Key_W:
        rotateUp();
        break;
       case Qt::Key_S:
        rotateDown();
        break;
       case Qt::Key_A:
        rotateLeft();
        break;
       case Qt::Key_D:
        rotateRight();
        break;
       case Qt::Key_X:
        translateUp();
        break;
       case Qt::Key_Z:
        translateDown();
        break;
       case Qt::Key_Space:
        defaultScene();
        break;
       case Qt::Key_Escape:
        this->close();
        break;
    }
    updateGL();
}

void letter::rotateUp()
{
    xRotate += 1.0;
}

void letter::rotateDown()
{
    xRotate -= 1.0;
}

void letter::rotateLeft()
{
    zRotate += 1.0;
}

void letter::rotateRight()
{
    zRotate -= 1.0;
}

void letter::translateDown()
{
    zTranslation -= 0.05;
}

void letter::translateUp()
{
    zTranslation += 0.05;
}

void letter::scalePlus()
{
    scale *= 1.1;
}

void letter::scaleMinus()
{
    scale /= 1.1;
}

void letter::defaultScene()
{
    xRotate = -90;
    yRotate = 0;
    zRotate = 0;
    zTranslation = 0;
    scale = 1;
}

void letter::drawAxis()
{
    glLineWidth(3.0f);
    glColor4f(1.00f, 0.00f, 0.00f, 1.0f);
    glBegin(GL_LINES);
    glVertex3f( 10.0f,  0.0f,  0.0f);
    glVertex3f(-10.0f,  0.0f,  0.0f);
    glEnd();
    QColor halfGreen(0, 128, 0, 255);
    qglColor(halfGreen);
    glBegin(GL_LINES);
    glVertex3f(0.0f,  10.0f,  0.0f);
    glVertex3f(0.0f, -10.0f,  0.0f);
    glColor4f(0.00f, 0.00f, 1.00f, 1.0f);
    glVertex3f(0.0f,  0.0f,  10.0f);
    glVertex3f(0.0f,  0.0f, -10.0f);
    glColor4f(0.00f, 0.00f, 0.00f, 0.0f);
    for (float i = -10; i < 10; i += 0.25)
    {
        float k = 20.0;
        glBegin(GL_LINES);
        glVertex3f(i, -1.0 / k, 0.0f);
        glVertex3f(i, 1.0 / k, 0.0f);
        glVertex3f(-1.0 / k, i, 0.0f);
        glVertex3f(1.0 / k, i, 0.0f);
        glVertex3f(0.0f, 1.0 / k, i);
        glVertex3f(0.0f, -1.0 / k, i);
    }
    glEnd();
}

void letter::drawFigure()
{
    glVertexPointer(3, GL_FLOAT, 0, vertexArr);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}

void letter::getVertexArray(float k)
{
    vertexArr[0][0] = 0;
    vertexArr[0][1] = 0;
    vertexArr[0][2] = 0;
    vertexArr[1][0] = 0;
    vertexArr[1][1] = 0;
    vertexArr[1][2] = -0.25 * k;
    vertexArr[2][0] = 0;
    vertexArr[2][1] = 2 * k;
    vertexArr[2][2] = -0.25 * k;
    vertexArr[3][0] = 0;
    vertexArr[3][1] = 2 * k;
    vertexArr[3][2] = 0;
    vertexArr[4][0] = 0;
    vertexArr[4][1] = 2 * k;
    vertexArr[4][2] = 0;
    vertexArr[5][0] = 0;
    vertexArr[5][1] = 2 * k;
    vertexArr[5][2] = -0.25 * k;
    vertexArr[6][0] = k;
    vertexArr[6][1] = 2 * k;
    vertexArr[6][2] = -0.25 * k;
    vertexArr[7][0] = k;
    vertexArr[7][1] = 2 * k;
    vertexArr[7][2] = 0;
    vertexArr[8][0] = 0;
    vertexArr[8][1] = 0;
    vertexArr[8][2] = 0;
    vertexArr[9][0] = 0;
    vertexArr[9][1] = 0;
    vertexArr[9][2] = -0.25 * k;
    vertexArr[10][0] = 0.25 * k;
    vertexArr[10][1] = 0;
    vertexArr[10][2] = -0.25 * k;
    vertexArr[11][0] = 0.25 * k;
    vertexArr[11][1] = 0;
    vertexArr[11][2] = 0;
    vertexArr[12][0] = 0;
    vertexArr[12][1] = 0;
    vertexArr[12][2] = 0;
    vertexArr[13][0] = 0.25 * k;
    vertexArr[13][1] = 0;
    vertexArr[13][2] = 0;
    vertexArr[14][0] = 0.25 * k;
    vertexArr[14][1] = 2 * k;
    vertexArr[14][2] = 0;
    vertexArr[15][0] = 0;
    vertexArr[15][1] = 2 * k;
    vertexArr[15][2] = 0;
    vertexArr[16][0] = 0;
    vertexArr[16][1] = 0;
    vertexArr[16][2] = -0.25 * k;
    vertexArr[17][0] = 0.25 * k;
    vertexArr[17][1] = 0;
    vertexArr[17][2] = -0.25 * k;
    vertexArr[18][0] = 0.25 * k;
    vertexArr[18][1] = 2 * k;
    vertexArr[18][2] = -0.25 * k;
    vertexArr[19][0] = 0;
    vertexArr[19][1] = 2 * k;
    vertexArr[19][2] = -0.25 * k;
    vertexArr[20][0] = k;
    vertexArr[20][1] = 1.75 * k;
    vertexArr[20][2] = 0;
    vertexArr[21][0] = k;
    vertexArr[21][1] = 1.75 * k;
    vertexArr[21][2] = 0;
    vertexArr[22][0] = k;
    vertexArr[22][1] = 1.75 * k;
    vertexArr[22][2] = -0.25 * k;
    vertexArr[23][0] = k;
    vertexArr[23][1] = 1.75 * k;
    vertexArr[23][2] = -0.25 * k;
    vertexArr[24][0] = 0.25 * k;
    vertexArr[24][1] = 1.75 * k;
    vertexArr[24][2] = 0;
    vertexArr[25][0] = 0.25 * k;
    vertexArr[25][1] = 1.75 * k;
    vertexArr[25][2] = -0.25 * k;
    vertexArr[26][0] = k;
    vertexArr[26][1] = 1.75 * k;
    vertexArr[26][2] = -0.25 * k;
    vertexArr[27][0] = k;
    vertexArr[27][1] = 1.75 * k;
    vertexArr[27][2] = 0;
    vertexArr[28][0] = 0.25 * k;
    vertexArr[28][1] = 2 * k;
    vertexArr[28][2] = 0;
    vertexArr[29][0] = 0.25 * k;
    vertexArr[29][1] = 1.75 * k;
    vertexArr[29][2] = 0;
    vertexArr[30][0] = k;
    vertexArr[30][1] = 1.75 * k;
    vertexArr[30][2] = 0;
    vertexArr[31][0] = k;
    vertexArr[31][1] = 2 * k;
    vertexArr[31][2] = 0;
    vertexArr[32][0] = 0.25 * k;
    vertexArr[32][1] = 2 * k;
    vertexArr[32][2] = -0.25 * k;
    vertexArr[33][0] = 0.25;
    vertexArr[33][1] = 1.75 * k;
    vertexArr[33][2] = -0.25 * k;
    vertexArr[34][0] = k;
    vertexArr[34][1] = 1.75 * k;
    vertexArr[34][2] = -0.25 * k;
    vertexArr[35][0] = k;
    vertexArr[35][1] = 2 * k;
    vertexArr[35][2] = -0.25 * k;
}

void letter::getIndexArray()
{
    idxArr[0][0] = 0;
    idxArr[0][1] = 1;
    idxArr[0][2] = 2;
    idxArr[0][3] = 3;
    idxArr[1][0] = 4;
    idxArr[1][1] = 5;
    idxArr[1][2] = 6;
    idxArr[1][3] = 7;
    idxArr[2][0] = 8;
    idxArr[2][1] = 9;
    idxArr[2][2] = 10;
    idxArr[2][3] = 11;
    idxArr[3][0] = 12;
    idxArr[3][1] = 13;
    idxArr[3][2] = 14;
    idxArr[3][3] = 15;
    idxArr[4][0] = 16;
    idxArr[4][1] = 17;
    idxArr[4][2] = 18;
    idxArr[4][3] = 19;
    idxArr[5][0] = 20;
    idxArr[5][1] = 21;
    idxArr[5][2] = 22;
    idxArr[5][3] = 23;
    idxArr[6][0] = 24;
    idxArr[6][1] = 25;
    idxArr[6][2] = 26;
    idxArr[6][3] = 27;
    idxArr[7][0] = 28;
    idxArr[7][1] = 29;
    idxArr[7][2] = 30;
    idxArr[7][3] = 31;
    idxArr[8][0] = 32;
    idxArr[8][1] = 33;
    idxArr[8][2] = 34;
    idxArr[8][3] = 35;
}

void letter::rotationX()
{
    GLfloat c[36][3];
    GLfloat conv[3][3] ;
    conv[0][0] = 1;
    conv[0][1] = 0;
    conv[0][2] = 0;
    conv[1][0] = 0;
    conv[1][1] = cos(angle * (M_PI / 180.0));
    conv[1][2] = -sin(angle * (M_PI / 180.0));
    conv[2][0] = 0;
    conv[2][1] = sin(angle * (M_PI / 180.0));
    conv[2][2] = cos(angle * (M_PI / 180.0));
    qDebug() << "Матрица вращения вокруг оси X" << endl;
    for (int i = 0; i < 3; i++)
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" "<<endl;

    for (int i = 0; i < 36; i++)
        for (int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for(int k = 0; k < 3; k++)
                c[i][j] += vertexArr[i][k] * conv[k][j];
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}

void letter::rotationY()
{
    GLfloat c[36][3];
    GLfloat conv[3][3] ;
    conv[0][0] = cos(angle * (M_PI / 180.0));
    conv[0][1] = 0;
    conv[0][2] = sin(angle * (M_PI / 180.0));
    conv[1][0] = 0;
    conv[1][1] = 1;
    conv[1][2] = 0;
    conv[2][0] = -sin(angle * (M_PI / 180.0));
    conv[2][1] = 0;
    conv[2][2] = cos(angle * (M_PI / 180.0));
    qDebug() << "Матрица вращения вокруг оси Y" << endl;
    for (int i = 0; i < 3; i++)
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" "<<endl;

    for (int i = 0; i < 36; i++)
        for (int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 3; k++)
                c[i][j] += vertexArr[i][k] * conv[k][j];
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);

}

void letter::rotationZ()
{
    GLfloat c[36][3];
    GLfloat conv[3][3] ;
    conv[0][0] = cos(angle * (M_PI / 180.0));;
    conv[0][1] = -sin(angle * (M_PI / 180.0));;
    conv[0][2] = 0;
    conv[1][0] = sin(angle * (M_PI / 180.0));
    conv[1][1] = cos(angle * (M_PI / 180.0));
    conv[1][2] = 0;
    conv[2][0] = 0;
    conv[2][1] = 0;
    conv[2][2] = 1;
    qDebug() << "Матрица вращения вокруг оси Z" << endl;
    for (int i = 0; i < 3; i++)
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" "<<endl;
    for (int i = 0; i < 36; i++)
        for (int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 3; k++)
                c[i][j] += vertexArr[i][k] * conv[k][j];
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}

void letter::translation()
{
    GLfloat c[36][3];
    GLfloat conv[4][3];
    conv[0][0] = 1;
    conv[0][1] = 0;
    conv[0][2] = 0;
    conv[1][0] = 0;
    conv[1][1] = 1;
    conv[1][2] = 0;
    conv[2][0] = 0;
    conv[2][1] = 0;
    conv[2][2] = 1;
    conv[3][0] = xTransfer;
    conv[3][1] = yTransfer;
    conv[3][2] = zTransfer;
    qDebug() << "Матрица переноса" << endl;
    for (int i = 0; i < 4; i++)
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" "<<endl;
    for (int i = 0; i < 36; i++)
        for(int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 4; k++)
            {
                if (k == 3)
                    c[i][j] += 1 * conv[k][j];
                else
                    c[i][j] += vertexArr[i][k] * conv[k][j];
            }
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}

void letter::scaling()
{
    GLfloat c[36][3];
    GLfloat conv[3][3];
    conv[0][0] = xScale;
    conv[0][1] = 0;
    conv[0][2] = 0;
    conv[1][0] = 0;
    conv[1][1] = yScale;
    conv[1][2] = 0;
    conv[2][0] = 0;
    conv[2][1] = 0;
    conv[2][2] = zScale;
    qDebug() << "Матрица масштабирования" << endl;
    for (int i = 0; i < 3; i++)
        qDebug()<<conv[i][0]<<" "<<conv[i][1]<<" "<<conv[i][2]<<" "<<endl;
    for (int i = 0; i < 36; i++)
        for(int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 3; k++)
                c[i][j] += vertexArr[i][k] * conv[k][j];
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}

void letter::projectionXY()
{
    GLfloat c[36][3];
    GLfloat conv[3][3] ;
    conv[0][0] = 1;
    conv[0][1] = 0;
    conv[0][2] = 0;
    conv[1][0] = 0;
    conv[1][1] = 1;
    conv[1][2] = 0;
    conv[2][0] = 0;
    conv[2][1] = 0;
    conv[2][2] = 0;
    for (int i = 0; i < 36; i++)
        for (int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 3; k++)
                c[i][j] += vertexArr[i][k] * conv[k][j];
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}

void letter::projectionXZ()
{
    GLfloat c[36][3];
    GLfloat conv[3][3] ;
    conv[0][0] = 1;
    conv[0][1] = 0;
    conv[0][2] = 0;
    conv[1][0] = 0;
    conv[1][1] = 0;
    conv[1][2] = 0;
    conv[2][0] = 0;
    conv[2][1] = 0;
    conv[2][2] = 1;
    for (int i = 0; i < 36; i++)
        for (int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 3; k++)
                c[i][j] += vertexArr[i][k] * conv[k][j];
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}

void letter::projectionYZ()
{
    GLfloat c[36][3];
    GLfloat conv[3][3] ;
    conv[0][0] = 0;
    conv[0][1] = 0;
    conv[0][2] = 0;
    conv[1][0] = 0;
    conv[1][1] = 1;
    conv[1][2] = 0;
    conv[2][0] = 0;
    conv[2][1] = 0;
    conv[2][2] = 1;
    for (int i = 0; i < 36; i++)
        for (int j = 0; j < 3; j++)
        {
            c[i][j] = 0;
            for (int k = 0; k < 3; k++)
                c[i][j] += vertexArr[i][k] * conv[k][j];
        }
    glVertexPointer(3, GL_FLOAT, 0, c);
    glDrawElements(GL_QUADS, 36, GL_UNSIGNED_BYTE, idxArr);
}
