#include "camera.h"
#include "ray.h"
#include "solid.h"
#include "sphere.h"

int main() {
  // camera positioning
  std::vector<int> sensor_size = {300, 300};
  std::vector<float> frame_size = {4, 4, 0};
  std::vector<float> frame_center = {0, 0, -1};
  Camera camera(sensor_size, frame_size, frame_center);

  // add one sphere to the scene
  std::vector<float> center = {0, 0, 2};
  std::vector<float> dimensions = {1.5};
  Sphere sphere(center, dimensions);
  std::vector<Sphere> spheres = {sphere};

  // magic
  camera.render(spheres);

  return 0;
}