#include "texture.hpp"

Texture::Texture()
  : texture (0)
{
}

Texture::Texture(const QString filePath)
  : texture(0)
{
  load(filePath);
}

Texture::~Texture(){
  glDeleteTextures(1, &texture);
}

GLuint &Texture::getTexture(){
  return texture;
}

Texture::operator GLuint() {
  return texture;
}

bool Texture::load(const QString filePath){
  QImage t, b;

  bool success = b.load(filePath);
  if (!success){
    qDebug() << "Error while loading texture from file" << filePath;
    b = QImage(16, 16, QImage::Format_RGB32);
    b.fill(QColor(0,255,0));
  }

  t = QGLWidget::convertToGLFormat( b );

  glGenTextures(1, &texture );
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits() );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

  return success;
}
