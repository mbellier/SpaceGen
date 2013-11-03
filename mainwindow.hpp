#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "glwidget.hpp"
#include "fpscamera.hpp"
#include "starfield.hpp"

#include <QGLBuffer>

#include "icosphere.hpp"

class MainWindow : public GLWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    void initializeGL();
    void resizeGL(int width, int height);
    void paintGL();
    void update();
    void keyPressEvent(QKeyEvent *keyEvent);
    void keyReleaseEvent(QKeyEvent *keyEvent);
    void processKeys();
private:
    int x;
    int mouseX;     // horizontal position of the mouse
    int mouseY;     // hertical position of the mouse
    int prevMouseX; // previous event horizontal position of the mouse
    int prevMouseY; // previous event vertical position of the mouse

    FPSCamera camera;
    StarField starField;

    QSet<Qt::Key> pressedKeys;

    Icosphere icosphere;

    // test vbo
    QVector<QVector3D>  m_vertices;
    QGLBuffer   m_vertexbuffer;
    QGLBuffer   m_indicebuffer;

    // test shader
};

#endif // MYWINDOW_H
