#ifndef SOLID_H
#define SOLID_H

#include "ray.h"
#include <tuple>
#include <vector>

class Solid {
public:
  Solid(const std::vector<float> &center, const std::vector<float> &dimensions)
      : _center(center), _dimensions(dimensions){};

  std::vector<float> getCenter() { return _center; };
  std::vector<float> getDimensions() { return _dimensions; };

  float hitBy(const Ray &ray);
  std::vector<float> normalAt(const std::vector<float> &point);

protected:
  std::vector<float> _center;
  std::vector<float> _dimensions;
};

#endif
