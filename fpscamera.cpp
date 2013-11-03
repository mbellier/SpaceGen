#include "fpscamera.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
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

//void FPSCamera::setViewByMouse(int mouseX, int mouseY, int width, int height){


//  // the middle of the screen in the x direction
//  int middleX = width/2;

//  // the middle of the screen in the y direction
//  int middleY = height/2;

//  // vector that describes mouseposition - center
//  glm::vec3 mouseDirection(0, 0, 0);

//  // static variable to store the rotation about the x-axis, since
//  // we want to limit how far up or down we can look.
//  // We don't need to cap the rotation about the y-axis as we
//  // want to be able to turn around 360 degrees
//  static double currentRotationX = 0.0;
////  static double currentRotationY = 0.0;


//  // The maximum angle we can look up or down, in radians
//  double maxAngle = 1;

//  // if the mouse hasn't moved, return without doing
//  // anything to our view
//  if((mouseX == middleX) && (mouseY == middleY))
//    return;

//  // otherwise move the mouse back to the middle of the screen
////  SDL_WarpMouse(middleX, middleY);

//  float mouseSensitivity = 500;
//  mouseDirection.x = (middleX - mouseX) / mouseSensitivity;
//  mouseDirection.y = (middleY - mouseY) / mouseSensitivity;

//  currentRotationX += mouseDirection.x;


////  if(currentRotationX > maxAngle){
////    currentRotationX = maxAngle;
////    return;
////  }
////  if(currentRotationX < -maxAngle){
////    currentRotationX = -maxAngle;
////    return;
////  }


//  // get the axis to rotate around the x-axis.
//  glm::vec3 axis = glm::cross(view - pos, up);
//  // To be able to use the quaternion conjugate, the axis to
//  // rotate around must be normalized.
//  axis = glm::normalize(axis);

//  // Rotate around the y axis
//  rotate(mouseDirection.y, axis.x, axis.y, axis.z);
//  // Rotate around the x axis
//  rotate(mouseDirection.x, 0, 1, 0);
//}



