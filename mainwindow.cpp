#include "mainwindow.hpp"

#ifdef _WIN32
#include <windows.h>
#include <gl/GLU.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif


#include <QtOpenGL>
#include <QCursor>
#include <QTime>
#include <QDebug>

#include <glm/gtx/constants.hpp>

#include "starfield.hpp"

#define PI 3.1416

#define gluPerspective(fovyInDegrees,aspectRatio,zNear,zFar)\
{float ymax = zNear * glm::tan(fovyInDegrees * glm::pi<float>() / 360.0);\
    float xmax = ymax * aspectRatio;\
    glFrustum(-xmax, xmax, -ymax, ymax, zNear, zFar);}


MainWindow::MainWindow(QWidget *parent)
  : GLWidget(60, parent, "Test OpenGL"),
    x(0), mouseX(0), mouseY(0), prevMouseX(0), prevMouseY(0),
    cameraSpeed(.2f),m_shaderProgram(new QGLShaderProgram(this)),
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

void MainWindow::closeEvent(QCloseEvent *event)
{
  if (icosphere_vertexBuffer != NULL){
    icosphere_vertexBuffer->release();
    delete icosphere_vertexBuffer;
  }

  event->accept();
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


  icosphere.create(7);
  qDebug() << "Vertices: " << icosphere.geometry().size() / 3;


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

  // icosphere as vertex array
  QVector<float> array;
  for (int i=0; i < 1; i++){
    for (int j=0; j < 1; j++){
      for (int h=0; h < 1; h++){
        for (int k=0; k < icosphere.geometry().size(); k++){
          array.append(icosphere.geometry().at(k) + ((k%3==0) ? i : j) + ((k%3==1)?j:0) + ((k%3==2)?h:0));
        }
      }
    }
  }
  icosphere_vertexBuffer = new QGLBuffer(QGLBuffer::VertexBuffer);
  icosphere_vertexBuffer->create();
  icosphere_vertexBuffer->bind();
  icosphere_vertexBuffer->setUsagePattern(QGLBuffer::StaticDraw);
  icosphere_vertexBuffer->allocate(array.constData(), array.size() * sizeof(float));

  /*
  // test shader
  m_shaderProgram.addShaderFromSourceCode(QGLShader::Vertex,
  "#version 130\nin highp vec4 vertex;in mediump mat4 matrix;\nvoid main(void)\n{\n gl_Position = matrix * vertex;\n}");
  m_shaderProgram.addShaderFromSourceCode(QGLShader::Fragment,
  "#version 130\n uniform mediump vec4 color;\nout mediump vec4 out_color;\nvoid main(void)\n{\n   out_color = color;\n}");
  m_shaderProgram.link();
  m_shaderProgram.bind();
*/
    qDebug() << m_shaderProgram.log();

  fpsTimer.start();
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

  glm::mat4 m = glm::mat4_cast(camera.getOrientation());
  glMultMatrixf((const GLfloat*)&m);
  glTranslatef(camera.getPosition().x, camera.getPosition().y, camera.getPosition().z);


/*
  // test shader ////////////////////////////////////////////////////////
  int vertexLocation = m_shaderProgram.attributeLocation("vertex");
  int matrixLocation = m_shaderProgram.attributeLocation("matrix");
  int colorLocation = m_shaderProgram.uniformLocation("color");

  QVector3D triangleVertices[] = {
     QVector3D(60.0f,  10.0f,  0.0f),
     QVector3D(110.0f, 110.0f, 0.0f),
     QVector3D(10.0f,  110.0f, 0.0f)
  };

  QMatrix4x4 pmvMatrix;
  pmvMatrix.ortho(QRect(2,1,1,1));

  m_shaderProgram.enableAttributeArray(vertexLocation);
  m_shaderProgram.setAttributeArray(vertexLocation, triangleVertices);
  m_shaderProgram.setUniformValue(matrixLocation, pmvMatrix);
  m_shaderProgram.setUniformValue(colorLocation, QColor(0, 255, 0, 255));


  glEnableClientState(GL_VERTEX_ARRAY);
  glDrawArrays(GL_TRIANGLES, 0, 3);
  glDisableClientState(GL_VERTEX_ARRAY);

  m_shaderProgram.disableAttributeArray(vertexLocation);
*/



  // draw test icosphere ////////////////////////////////////////////////

//  glBegin(GL_TRIANGLES);
//  for (int i = 0; i < icosphere.geometry().size(); i += 3){
//    glColor3ub( i%255, i%255, i%255 );
//    glVertex3f(icosphere.geometry().at(i),icosphere.geometry().at(i+1),icosphere.geometry().at(i+2));
//  }
//  glEnd();

//  glEnableClientState(GL_VERTEX_ARRAY);
//  glTranslatef(1,1,1);
//  if (icosphere_vertexBuffer->bind ()) {
//    glVertexPointer( 3, GL_FLOAT, 0, NULL );;
//     glDrawArrays( GL_TRIANGLES, 0,icosphere_vertexBuffer->size());
//  }
//  glTranslatef(-1,-1,-1);
//  glDisableClientState(GL_VERTEX_ARRAY);



  // test VBO
//  qglColor(Qt::white);
//  glEnableClientState(GL_VERTEX_ARRAY);
//  glVertexPointer(3, GL_FLOAT, 0, m_vertices.constData());
//  glDrawArrays(GL_TRIANGLES, 0, m_vertices.size());
//  glDisableClientState(GL_VERTEX_ARRAY);


  // draw frame ////////////////////////////////////////////////////////
  const float length = 5000.f;
  glBegin(GL_LINES);
  glColor3f(1.,1.,1.);
  glVertex3f( length,0.0f,0.0f);
  glVertex3f(-length,0.0f,0.0f);
  glVertex3f(0.0f,-length,0.0f);
  glVertex3f(0.0f,length,0.0f);
  glVertex3f(0.0f,0.0f,-length);
  glVertex3f(0.0f,0.0f,length);
  glEnd();


  // Draw star field
  starField.paintGL(camera);


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

unsigned int MainWindow::calculateFPS(){
  static unsigned int frameNumber = 1;
  static unsigned int fps = 0;
  unsigned int msec = fpsTimer.elapsed();
  if (msec < 1000) {
    frameNumber++;
  }
  else {
    fps = frameNumber * (1000 / (float)msec);
    frameNumber = 0;
    fpsTimer.restart();
  }
  return fps;
}

void MainWindow::update()
{

  this->setWindowTitle(QString::number(calculateFPS()));

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

  const float angle = 2.;

  for (QSet<Qt::Key>::const_iterator key = pressedKeys.cbegin(); key != pressedKeys.cend(); key++)

  switch(*key){
  case Qt::Key_Escape:
    this->close();
    break;
  case Qt::Key_Z:
    camera.moveForward(cameraSpeed);
    break;
  case Qt::Key_S:
    camera.moveForward(-cameraSpeed);
    break;
  case Qt::Key_Q:
    camera.moveRight(cameraSpeed);
    break;
  case Qt::Key_D:
    camera.moveRight(-cameraSpeed);
    break;

  case Qt::Key_E:
     camera.rotate(angle, 0, 0, 1);
    break;
  case Qt::Key_A:
    camera.rotate(-angle, 0, 0, 1);
    break;

  case Qt::Key_Space:
    camera.moveUpward(cameraSpeed);
    break;
  case Qt::Key_Shift:
    camera.moveUpward(-cameraSpeed);
    break;

  case Qt::Key_Up:
    camera.rotate(angle, 1, 0, 0);
    break;
  case Qt::Key_Down:
    camera.rotate(-angle, 1, 0, 0);
    break;
  case Qt::Key_Left:
    camera.rotate(-angle, 0, 0, 1);
    break;
  case Qt::Key_Right:
    camera.rotate(angle, 0, 0, 1);
    break;

  case Qt::Key_Plus:
      cameraSpeed += .1f;
     break;
  case Qt::Key_Minus:
      cameraSpeed -= .1f;
      if (cameraSpeed < .1f)
          cameraSpeed = .1f;
      break;
  default:
      break;
  }
}


