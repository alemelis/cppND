#ifndef RAY_H
#define RAY_H

#include <vector>

class Ray {
public:
  Ray(std::vector<float> &source, std::vector<float> &target);

  std::vector<float> getSource() { return _source; };
  std::vector<float> getDirection() { return _direction; };
  std::vector<float> travel(float distance);
  float distanceFrom(const std::vector<float> &point);

private:
  std::vector<float> _source;
  std::vector<float> _direction;
  std::vector<float> _target;
};

#endif