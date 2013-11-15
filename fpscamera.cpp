#include "fpscamera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <QDebug>

FPSCamera::FPSCamera()
  : pos(0,0,0), orient(glm::vec3(0,0,0))
{
}

void FPSCamera::move(float distance, glm::vec3 axis){
  pos += (distance * axis) * orient;
}

void FPSCamera::moveForward(float distance){
//  pos += glm::vec3(0, 0, distance ) * orient;
  move(distance, glm::vec3(0,0,1));
}

void FPSCamera::moveRight(float distance){
  pos += glm::vec3(distance, 0, 0) * orient;
}

void FPSCamera::moveUpward(float distance){
  move(-distance, glm::vec3(0,1,0));
}

void FPSCamera::setPosition(glm::vec3 position){
  pos = position;
}

void FPSCamera::rotate(float angle, float x, float y, float z){
  glm::quat rot = glm::angleAxis(angle, x, y, z);
  orient = glm::normalize(rot * orient);
}

glm::quat FPSCamera::getOrientation() const {
  return orient;
}

glm::vec3 FPSCamera::getPosition() const{
  return pos;
}



