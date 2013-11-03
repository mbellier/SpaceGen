#include "starfield.hpp"

#include <stdlib.h>

#include <QtOpenGL>
#include <QImage>

#ifdef WINDOWS
#include <windows.h> // required on Windows for GL

#include "gl/GLU.h"
#include "gl/GL.h"
#endif

#include "morton.hpp"
#include "texture.hpp"


StarField::StarField()
{
}





void StarField::initGL()
{


  glEnable( GL_POINT_SPRITE_ARB );
  glEnable( GL_POINT_SMOOTH );
  glEnable( GL_BLEND );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

  glPointSize( 64.0 );

  tex.load("assets\\textures\\particle.png");
  glBindTexture(GL_TEXTURE_2D, tex);
  glEnable(GL_POINT_SPRITE);
  glTexEnvi(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);




}

void StarField::paintGL()
{

  // cf. GL_POINT_SPRITE_ARB ?
  // cf. VBO ?
  glEnable(GL_TEXTURE_2D);
  glBegin(GL_POINTS);

  int range = 20;
  for (int x = -range; x < range; x++){
    for (int y = -range; y < range; y++){
      for (int z = -range; z < range; z++){
        srand(Morton::signed_encode_3(x,y,z));
        glColor3ub( rand()%255, rand()%255, rand()%255 );
//        glColor4f(1,1,1,1);
        glVertex3f(x*2, y*2, z*2);


      }
    }
  }

  glEnd();
  glDisable(GL_TEXTURE_2D);
}
