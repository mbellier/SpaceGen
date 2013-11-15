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
  icosphere.create(0);
}


StarField::~StarField(){
  if (icosphere_vertexBuffer != NULL){
    icosphere_vertexBuffer->release();
    delete icosphere_vertexBuffer;
  }
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

// single icosphere vbo

//  icosphere_vertexBuffer = new QGLBuffer(QGLBuffer::VertexBuffer);
//  icosphere_vertexBuffer->create();
//  icosphere_vertexBuffer->bind();
//  icosphere_vertexBuffer->setUsagePattern(QGLBuffer::StaticDraw);
//  icosphere_vertexBuffer->allocate(icosphere.geometry().constData(), icosphere.geometry().size() * sizeof(float));


  // multiple icospheres vbo
  icosphere_vertexBuffer = new QGLBuffer(QGLBuffer::VertexBuffer);
  icosphere_vertexBuffer->create();
  icosphere_vertexBuffer->bind();
  icosphere_vertexBuffer->setUsagePattern(QGLBuffer::StaticDraw);


  const int   range = 10;
  const int   step  = 80;
  const float scale = .5f;
  for (int x = -range; x < range; x++){
    for (int y = -range; y < range; y++){
      for (int z = -range; z < range; z++){
        srand(Morton::signed_encode_3(x,y,z));

        float radius = .5 +(rand()%100/100.) * scale,
            px=(x+rand()%100/100.)*step,
            py=(y+rand()%100/100.)*step,
            pz=(z+rand()%100/100.)*step;

        for (int k=0; k < icosphere.geometry().size(); k+=3){
            array.append(icosphere.geometry().at(k) + px);
            array.append(icosphere.geometry().at(k+1) + py);
            array.append(icosphere.geometry().at(k+2) + pz);
        }

      }
    }
  }

  icosphere_vertexBuffer->allocate(array.constData(), array.size() * sizeof(float));




}

void StarField::paintGL(FPSCamera &cam)
{

  // cf. GL_POINT_SPRITE_ARB ?


//  glEnableClientState(GL_VERTEX_ARRAY);
//  icosphere_vertexBuffer->bind ();
//  glVertexPointer( 3, GL_FLOAT, 0, NULL );
//  const int   range = 10;
//  const int   step  = 80;
//  const float scale = .5f;
//  for (int x = -range; x < range; x++){
//    for (int y = -range; y < range; y++){
//      for (int z = -range; z < range; z++){
//        srand(Morton::signed_encode_3(x,y,z));
//        glColor3ub( rand()%255, rand()%255, rand()%255 );

//        float radius = .5 +(rand()%100/100.) * scale,
//            px=(x+rand()%100/100.)*step,
//            py=(y+rand()%100/100.)*step,
//            pz=(z+rand()%100/100.)*step;

//        glTranslatef(px,py,pz);
//        glScalef(radius, radius, radius);
//        glDrawArrays( GL_TRIANGLES, 0,icosphere.geometry().size() );
//        glScalef(1./radius, 1./radius, 1./radius);
//        glTranslatef(-px,-py,-pz);

//      }
//    }
//  }
//  glDisableClientState(GL_VERTEX_ARRAY);


    glEnableClientState(GL_VERTEX_ARRAY);
    icosphere_vertexBuffer->bind ();
    glVertexPointer( 3, GL_FLOAT, 0, NULL );
    glDrawArrays( GL_TRIANGLES, 0, array.size());
    glDisableClientState(GL_VERTEX_ARRAY);

}
