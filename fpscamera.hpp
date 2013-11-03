#ifndef FPSCAMERA_H
#define FPSCAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class FPSCamera
{
public:
  FPSCamera();
  void      setPosition    (glm::vec3 position);
  glm::vec3 getPosition    () const;
  void      moveForward    (float distance);
  void      moveRight      (float distance);
  void      rotate         (float angle, float x, float y, float z);
  glm::quat getOrientation () const;
  void      setViewByMouse (int mouseX, int mouseY, int width, int height);

  void move(float distance, glm::vec3 axis);
  void moveUpward(float distance);
private:
  glm::vec3 pos;
  glm::quat orient;
};

#endif // FPSCAMERA_H
