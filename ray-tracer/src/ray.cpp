#include "ray.h"
#include <math.h>

Ray::Ray(std::vector<float> &source, std::vector<float> &target) {
  _source = source;
  _target = target;
  float distance = distanceFrom(target);
  for (unsigned int i = 0; i < 3; i++) {
    _direction.push_back(abs(_source[i] - target[i]) / distance);
  }
}

float Ray::distanceFrom(const std::vector<float> &point) {
  float distance = 0.0;
  for (unsigned int i = 0; i < 3; i++) {
    distance += pow(_source[i] - point[i], 2);
  }
  return sqrt(distance);
}

std::vector<float> Ray::travel(float distance) {
  std::vector<float> destination;
  for (unsigned int i = 0; i < 3; i++) {
    destination.push_back(_source[i] + _direction[i] * distance);
  }
  return destination;
}