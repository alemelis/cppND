#ifndef CAMERA_H
#define CAMERA_H

#include "sphere.h"
#include <vector>

class Camera {
public:
  Camera() {}
  Camera(const std::vector<int> sensor_size,
         const std::vector<float> frame_size,
         const std::vector<float> frame_center);

  Ray castRay(float i, float j);

  void render(std::vector<Sphere> &objects);

private:
  std::vector<int> _sensor_size;
  std::vector<float> _frame_size;
  std::vector<float> _frame_center;
  std::vector<float> _source;
  std::vector<float> _bottom_left_corner;
  std::vector<float> _frame_sensor_ratio;
};

#endif