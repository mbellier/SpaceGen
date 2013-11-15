#include "glwidget.hpp"

GLWidget::GLWidget(int framesPerSecond, QWidget *parent, const QString &name)
    : QGLWidget(parent)
{
    setWindowTitle(name);
    if(framesPerSecond == 0)
        t_Timer = NULL;
    else
    {
        int ms = 1000;
        int timerInterval = ms / framesPerSecond;
        t_Timer = new QTimer(this);
        connect(t_Timer, SIGNAL(timeout()), this, SLOT(timeOutSlot()));
        t_Timer->start( timerInterval );
    }
}

void GLWidget::keyPressEvent(QKeyEvent *keyEvent)
{
    switch(keyEvent->key())
    {
        case Qt::Key_Escape:
            close();
            break;
    }
}

void GLWidget::timeOutSlot()
{
  updateGL();
}
