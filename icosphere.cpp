#include "icosphere.hpp"

#include <QDebug>

#include "morton.hpp"
#include <QtOpenGL>

Icosphere::Icosphere()
{
}


// add vertex to mesh, fix position to be on unit sphere, return index
int Icosphere::addVertex(glm::vec3 p)
{
  double length = glm::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
  geometry_positions.push_back(glm::vec3(p.x/length, p.y/length, p.z/length));
  return index++;
}

// return index of point in the middle of p1 and p2
int Icosphere::getMiddlePoint(int p1, int p2)
{
  // first check if we have it already
  bool firstIsSmaller = p1 < p2;
  int smallerIndex = firstIsSmaller ? p1 : p2;
  int greaterIndex = firstIsSmaller ? p2 : p1;
//  int key = (smallerIndex << 32) + greaterIndex;
//  int key[2] = {smallerIndex, greaterIndex};
  unsigned int key = Morton::encode_2(smallerIndex, greaterIndex);

  if (middlePoints.contains(key))
      return middlePoints.value(key);

  // not in cache, calculate it
  glm::vec3 point1 = geometry_positions[p1];
  glm::vec3 point2 = geometry_positions[p2];
  glm::vec3 middle ((point1.x + point2.x) / 2.0,
                    (point1.y + point2.y) / 2.0,
                    (point1.z + point2.z) / 2.0);

  // add vertex makes sure point is on unit sphere
  int i = addVertex(middle);

  // store it, return index
  middlePoints.insert(key, i);
  return i;
}



void Icosphere::create(int recursionLevel)
{
//  this.geometry = new MeshGeometry3D();
//  this.middlePointIndexCache = new Dictionary<long, int>();
  this->index = 0;
  geometry_positions.clear();

  // create 12 vertices of a icosahedron
  float t = (1.0 + glm::sqrt(5.0)) / 2.0;

  addVertex(glm::vec3(-1,  t,  0));
  addVertex(glm::vec3( 1,  t,  0));
  addVertex(glm::vec3(-1, -t,  0));
  addVertex(glm::vec3( 1, -t,  0));

  addVertex(glm::vec3( 0, -1,  t));
  addVertex(glm::vec3( 0,  1,  t));
  addVertex(glm::vec3( 0, -1, -t));
  addVertex(glm::vec3( 0,  1, -t));

  addVertex(glm::vec3( t,  0, -1));
  addVertex(glm::vec3( t,  0,  1));
  addVertex(glm::vec3(-t,  0, -1));
  addVertex(glm::vec3(-t,  0,  1));


  // create 20 triangles of the icosahedron
  QVector<TriangleIndices> faces;
  faces.clear();

  // 5 faces around point 0
  faces.push_back(TriangleIndices(0, 11, 5));
  faces.push_back(TriangleIndices(0, 5, 1));
  faces.push_back(TriangleIndices(0, 1, 7));
  faces.push_back(TriangleIndices(0, 7, 10));
  faces.push_back(TriangleIndices(0, 10, 11));

  // 5 adjacent faces
  faces.push_back(TriangleIndices(1, 5, 9));
  faces.push_back(TriangleIndices(5, 11, 4));
  faces.push_back(TriangleIndices(11, 10, 2));
  faces.push_back(TriangleIndices(10, 7, 6));
  faces.push_back(TriangleIndices(7, 1, 8));

  // 5 faces around point 3
  faces.push_back(TriangleIndices(3, 9, 4));
  faces.push_back(TriangleIndices(3, 4, 2));
  faces.push_back(TriangleIndices(3, 2, 6));
  faces.push_back(TriangleIndices(3, 6, 8));
  faces.push_back(TriangleIndices(3, 8, 9));

  // 5 adjacent faces
  faces.push_back(TriangleIndices(4, 9, 5));
  faces.push_back(TriangleIndices(2, 4, 11));
  faces.push_back(TriangleIndices(6, 2, 10));
  faces.push_back(TriangleIndices(8, 6, 7));
  faces.push_back(TriangleIndices(9, 8, 1));


  // refine triangles
  for (int i = 0; i < recursionLevel; i++){
    QVector<TriangleIndices> faces2;
    for (QVector<TriangleIndices>::const_iterator tri = faces.cbegin();
         tri != faces.cend(); tri++)
    {
      // replace triangle by 4 triangles
      int a = getMiddlePoint((*tri).v1, (*tri).v2);
      int b = getMiddlePoint((*tri).v2, (*tri).v3);
      int c = getMiddlePoint((*tri).v3, (*tri).v1);

      faces2.push_back(TriangleIndices((*tri).v1, a, c));
      faces2.push_back(TriangleIndices((*tri).v2, b, a));
      faces2.push_back(TriangleIndices((*tri).v3, c, b));
      faces2.push_back(TriangleIndices(a, b, c));
    }
    faces = faces2;
  }


  geometry_triangleIndices.clear();
  // done, now add triangles to mesh
  for (QVector<TriangleIndices>::const_iterator tri = faces.cbegin(); tri != faces.cend(); tri++)
  {
    geometry_triangleIndices.push_back((*tri).v1);
    geometry_triangleIndices.push_back((*tri).v2);
    geometry_triangleIndices.push_back((*tri).v3);
  }

  _geometry.clear();
  for (QVector<int>::const_iterator it = geometry_triangleIndices.cbegin();
       it != geometry_triangleIndices.cend();
       it++)
  {
    glm::vec3 pos = geometry_positions.at((*it));
    _geometry.push_back(pos.x);
    _geometry.push_back(pos.y);
    _geometry.push_back(pos.z);
  }



}

const QVector<float> &Icosphere::geometry() const
{
  return _geometry;
}

// TODO move to mesh class
void Icosphere::paint(float radius, float x, float y, float z) const
{
  glBegin(GL_TRIANGLES);
  for (int i = 0; i < geometry().size(); i += 3){
    glVertex3f(x + geometry().at(i)   * radius,
               y + geometry().at(i+1) * radius,
               z + geometry().at(i+2) * radius);
  }
  glEnd();
}


