#ifndef STARFIELD_H
#define STARFIELD_H

#include <QtOpenGL>
#include "texture.hpp"

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

//  GlslProg	mShader;
//  Texture		mTextureStar;
//  Texture		mTextureCorona;
//  VboMesh		mVboMesh;
};

#endif // STARFIELD_H
