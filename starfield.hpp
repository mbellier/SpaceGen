#ifndef STARFIELD_H
#define STARFIELD_H

#include <QtOpenGL>
#include "texture.hpp"
#include "icosphere.hpp"
#include "fpscamera.hpp"

class StarField
{
public:
  StarField();
  void initGL();
  void paintGL(FPSCamera &cam);
  GLuint *loadTexture(const QString filePath);
  ~StarField();
private:
  GLuint texture[1];

  Texture tex;

  Icosphere icosphere;
  QGLBuffer *icosphere_vertexBuffer;
    QVector<float> array;
};

#endif // STARFIELD_H
