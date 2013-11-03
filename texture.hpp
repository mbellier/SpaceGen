#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <QtOpenGL>

class Texture
{
public:
  Texture();
  Texture(const QString filePath);
  bool load(const QString filePath);
  GLuint &getTexture();

  operator GLuint();

  ~Texture();
private:
  GLuint texture;
};

#endif // TEXTURE_HPP
