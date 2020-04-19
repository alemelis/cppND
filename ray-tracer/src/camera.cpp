#include "camera.h"
#include "ray.h"
#include "solid.h"
#include <fstream>
#include <iostream>
#include <math.h>
#include <numeric>

Camera::Camera(const std::vector<int> sensor_size,
               const std::vector<float> frame_size,
               const std::vector<float> frame_center)
    : _sensor_size(sensor_size), _frame_size(frame_size),
      _frame_center(frame_center) {

  _source = {0.0, 0.0, 0.0};

  for (unsigned int i = 0; i < 3; i++) {
    _bottom_left_corner.push_back(_frame_center[i] - 0.5 * _frame_size[i]);
  }

  _frame_sensor_ratio.push_back(_frame_size[0] / _sensor_size[0]);
  _frame_sensor_ratio.push_back(_frame_size[1] / _sensor_size[1]);
}

Ray Camera::castRay(float i, float j) {
  std::vector<float> point;
  for (unsigned int k = 0; k < 3; k++) {
    point.push_back(_bottom_left_corner[k] - _source[k]);
  }
  point[0] += i * _frame_size[0];
  point[1] += j * _frame_size[1];
  return Ray(_source, point);
}

std::vector<float> normalise(std::vector<float> v) {
  float l = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
  v[0] /= l;
  v[1] /= l;
  v[2] /= l;
  return v;
}

void Camera::render(std::vector<Sphere> &objects) {
  int r, g, b;
  std::ofstream my_Image("rendering.ppm");
  if (my_Image.is_open()) {
    my_Image << "P3\n" << _sensor_size[0] << " " << _sensor_size[1] << " 255\n";
    for (int j = 0; j < _sensor_size[1]; j++) {
      for (int i = 0; i < _sensor_size[0]; i++) {

        Ray ray =
            castRay((float)i / _sensor_size[0], (float)j / _sensor_size[1]);

        for (auto object : objects) {
          float hit = object.hitBy(ray);
          if (hit != -1) {
            std::vector<float> P = ray.travel(hit);
            std::vector<float> N = normalise(object.normalAt(P));
            r = int(255 * 1.85 *
                    std::inner_product(N.begin(), N.end(), P.begin(), 0.0));
            g = 0;
            b = int(255 * 1.85 *
                    std::inner_product(N.begin(), N.end(), P.begin(), 0.0));;

          } else { // gradient background
            std::vector<float> p = ray.travel(1.0);
            std::vector<float> np = normalise(p);

            r = int(255 * np[0]);
            g = int(255 * np[2]);
            b = int(255 * np[1]);
          }
        }

        my_Image << r << ' ' << g << ' ' << b << "\n";
      }
    }
    my_Image.close();
  } else
    std::cout << "Could not open the file";
}