#ifndef SPHERE_H
#define SPHERE_H

#include "solid.h"

class Sphere : public Solid {
public:
  Sphere(const std::vector<float> center, const std::vector<float> dimensions)
      : Solid(center, dimensions){};

  float hitBy(Ray &ray);
  std::vector<float> normalAt(const std::vector<float> &point);
};

#endif
