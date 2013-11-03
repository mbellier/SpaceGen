#ifndef STARFIELD_H
#define STARFIELD_H

#include <QtOpenGL>
#include "texture.hpp"
#include "icosphere.hpp"

class StarField
{
public:
  StarField();
  void initGL();
  void paintGL();
  GLuint *loadTexture(const QString filePath);
private:
  GLuint texture[1];

  Texture tex;

  Icosphere icosphere;
};

#endif // STARFIELD_H
