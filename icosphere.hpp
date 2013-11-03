#ifndef ICOSPHERE_HPP
#define ICOSPHERE_HPP

#include <glm/glm.hpp>
#include <QVector>
#include <QMap>

class Icosphere
{
public:
  Icosphere();
  void create(int recursionLevel);
  const QVector<float> &geometry() const;

  void paint(float radius, float x, float y, float z) const;

private:
  int addVertex(glm::vec3 p);
  int getMiddlePoint(int p1, int p2);

  struct TriangleIndices
  {
    int v1;
    int v2;
    int v3;
    TriangleIndices(){
      v1 = 0; v2 = 0; v3 = 0;
    }

    TriangleIndices(int v1, int v2, int v3)
    {
      this->v1 = v1;
      this->v2 = v2;
      this->v3 = v3;
    }
  };

  int index;
  QMap<unsigned int, int> middlePoints;
  QVector<int> geometry_triangleIndices;
  QVector<glm::vec3> geometry_positions;
  QVector<float> _geometry;
};


#endif // ICOSPHERE_HPP
