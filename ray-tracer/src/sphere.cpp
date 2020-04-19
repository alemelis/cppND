#include "sphere.h"
#include "ray.h"
#include <math.h>
#include <numeric>

// https://en.cppreference.com/w/cpp/algorithm/inner_product
// http://viclw17.github.io/2018/07/16/raytracing-ray-sphere-intersection/
float Sphere::hitBy(Ray &ray) {
  std::vector<float> ray_dir = ray.getDirection();
  std::vector<float> ray_src = ray.getSource();

  std::vector<float> source_to_center;
  for (unsigned int i = 0; i < 3; i++) {
    source_to_center.push_back(ray_src[i] - _center[i]);
  }

  float a =
      std::inner_product(ray_dir.begin(), ray_dir.end(), ray_dir.begin(), 0.0);
  float b =
      2.0 * std::inner_product(source_to_center.begin(), source_to_center.end(),
                               ray_dir.begin(), 0.0);
  float c = std::inner_product(source_to_center.begin(), source_to_center.end(),
                               source_to_center.begin(), 0.0) -
            _dimensions[0] * _dimensions[0];
  float delta = b * b - 4 * a * c;

  if (delta < 0) {
    return -1;
  } else {
    float num = -b - sqrt(delta);
    if (num > 0) {
      return num / (2 * a);
    }
    num = -b + sqrt(delta);
    if (num > 0) {
      return num / (2 * a);
    } else {
      return -1;
    }
  }
}

std::vector<float> Sphere::normalAt(const std::vector<float> &point) {
  std::vector<float> normal;
  for (unsigned int i = 0; i < 3; i++) {
    normal.push_back((point[i] - _center[i]) * (-1 / _dimensions[0]));
  }
  return normal;
}