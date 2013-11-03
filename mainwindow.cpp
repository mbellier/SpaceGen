#include "mainwindow.hpp"

//#ifdef WINDOWS
#include "gl/GLU.h"
//#endif
#include <QtOpenGL>

#include <QCursor>

#include <QDebug>


#include "starfield.hpp"

#define PI 3.1416



MainWindow::MainWindow(QWidget *parent)
  : GLWidget(60, parent, "Test OpenGL"),
    x(0), mouseX(0), mouseY(0), prevMouseX(0), prevMouseY(0),
    m_vertexbuffer(QGLBuffer::VertexBuffer),
    m_indicebuffer(QGLBuffer::IndexBuffer)
{
  m_vertices.push_back(QVector3D(0,0,0));
  m_vertices.push_back(QVector3D(1,0,0));
  m_vertices.push_back(QVector3D(1,1,0));
  m_vertices.push_back(QVector3D(0,1,0));
  m_vertices.push_back(QVector3D(0,1,1));
  m_vertices.push_back(QVector3D(0,0,1));

}

void MainWindow::initializeGL()
{

  starField.initGL();


  glShadeModel(GL_SMOOTH);
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
  glEnable(GL_PROGRAM_POINT_SIZE);


  icosphere.create(1);
  qDebug() << icosphere.geometry().size() << icosphere.geometry().size() / 3;


  //buffers
  // Vertex buffer init
  m_vertexbuffer.create();
  m_vertexbuffer.bind();
  m_vertexbuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(QVector3D));
  m_vertexbuffer.release();

  // Indices buffer init
  m_indicebuffer.create();
  m_indicebuffer.bind();
  m_indicebuffer.allocate(m_vertices.constData(), m_vertices.size() * sizeof(GLuint));
  m_indicebuffer.release();
}

void MainWindow::resizeGL(int width, int height)
{
  if(height == 0)
    height = 1;
  glViewport(0, 0, width, height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f, (GLfloat)width/(GLfloat)height, 0.1f, 1000.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void MainWindow::paintGL()
{
  update();

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  glMultMatrixf((GLfloat*)&glm::mat4_cast(camera.getOrientation()));
  glTranslatef(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);

  starField.paintGL();


  glBegin(GL_TRIANGLES);
  for (int i = 0; i < icosphere.geometry().size(); i += 3){
    glColor3ub( i%255, i%255, i%255 );
    glVertex3f(icosphere.geometry().at(i),icosphere.geometry().at(i+1),icosphere.geometry().at(i+2));
  }
  glEnd();

//  qglColor(Qt::white);
//  glEnableClientState(GL_VERTEX_ARRAY);
//  glVertexPointer(3, GL_FLOAT, 0, icosphere.geometry().constData());
//  glDrawArrays(GL_TRIANGLES, 0, icosphere.geometry().size());
//  glDisableClientState(GL_VERTEX_ARRAY);

  // test VBO
//  qglColor(Qt::white);
//  glEnableClientState(GL_VERTEX_ARRAY);
//  glVertexPointer(3, GL_FLOAT, 0, m_vertices.constData());
//  glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
//  glDisableClientState(GL_VERTEX_ARRAY);

  // draw frame
  glBegin(GL_LINES);
  glColor3f(1.,1.,1.);
  glVertex3f( 100.0f,0.0f,0.0f);
  glVertex3f(-100.0f,0.0f,0.0f);
  glVertex3f(0.0f,-100.0f,0.0f);
  glVertex3f(0.0f,100.0f,0.0f);
  glVertex3f(0.0f,0.0f,-100.0f);
  glVertex3f(0.0f,0.0f,100.0f);
  glEnd();

  glTranslatef(3.0f, 0.0f, -6.0f);
  glRotatef(x*PI, 1.,1,0);

  glBegin(GL_QUADS);
  glColor3f(0,0,1);
  glVertex3f(-1.0f, 1.0f, 0.0f);
  glVertex3f(-1.0f, -1.0f, 0.0f);
  glVertex3f(1.0f, -1.0f, 0.0f);
  glVertex3d(1.0f, 1.0f, 0.0f);
  glEnd();



}

void MainWindow::update()
{
  x++;

  processKeys();

  int mouseX = this->mapFromGlobal(QCursor::pos()).x();
  int mouseY = this->mapFromGlobal(QCursor::pos()).y();

  int diffX = mouseX - width() / 2;
  int diffY = mouseY - height() / 2;

  camera.rotate(diffX / 500., 0, 1, 0);
  camera.rotate(diffY / 500., 1, 0, 0);

}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent){
  pressedKeys += (Qt::Key) keyEvent->key();
}


void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent){
  pressedKeys -= (Qt::Key) keyEvent->key();
}

void MainWindow::processKeys(){
  const float speed = .2f;
  const float angle = 1.;

  for (QSet<Qt::Key>::const_iterator key = pressedKeys.cbegin(); key != pressedKeys.cend(); key++)

  switch(*key){
  case Qt::Key_Escape:
    this->close();
    break;
  case Qt::Key_Z:
    camera.moveForward(speed);
    break;
  case Qt::Key_S:
    camera.moveForward(-speed);
    break;
  case Qt::Key_Q:
    camera.moveRight(speed);
    break;
  case Qt::Key_D:
    camera.moveRight(-speed);
    break;

  case Qt::Key_E:
     camera.rotate(angle, 0, 0, 1);
    break;
  case Qt::Key_A:
    camera.rotate(-angle, 0, 0, 1);
    break;

  case Qt::Key_Space:
    camera.moveUpward(speed);
    break;
  case Qt::Key_Shift:
    camera.moveUpward(-speed);
    break;

  case Qt::Key_Up:
    camera.rotate(angle, 1, 0, 0);
    break;
  case Qt::Key_Down:
    camera.rotate(-angle, 1, 0, 0);
    break;
  case Qt::Key_Left:
    camera.rotate(-angle, 0, 1, 0);
    break;
  case Qt::Key_Right:
    camera.rotate(angle, 0, 1, 0);
    break;
  }
}


