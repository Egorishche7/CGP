#ifndef LETTER_H
#define LETTER_H

#include <QMainWindow>
#include <QtOpenGL>
#include <QGLWidget>
#include <QOpenGLFunctions>
#include <GL/gl.h>
#include <GL/glu.h>

class letter : public QGLWidget
{
public:
    letter(QWidget *parent = nullptr);
    virtual ~letter() = default;

    QPoint ptrMousePos;
    QColor colour;
    GLfloat xRotate;
    GLfloat yRotate;
    GLfloat zRotate;
    GLfloat zTranslation;
    GLfloat scale;
    float xTransfer = 1, yTransfer = 1, zTransfer = 1;
    float xScale = 1, yScale = 1, zScale = 1;
    int check = 0;
    float angle = 30;

    void rotateUp();
    void rotateDown();
    void rotateLeft();
    void rotateRight();
    void translateDown();
    void translateUp();
    void scalePlus();
    void scaleMinus();
    void defaultScene();
    void drawAxis();
    void drawFigure();
    void getVertexArray(float);
    void getIndexArray();
    void scaling();
    void translation();
    void rotationX();
    void rotationY();
    void rotationZ();
    void projectionXY();
    void projectionXZ();
    void projectionYZ();

    void initializeGL();
    void resizeGL(int, int);
    void paintGL();
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void wheelEvent(QWheelEvent*);
    void keyPressEvent(QKeyEvent*);
};
#endif // LETTER_H
