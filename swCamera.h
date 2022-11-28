#pragma once

#include "swRay.h"

namespace sw {

class Camera {
  public:
      Camera() {
        // Camera set up
        const float aspect_ratio = 16.0f / 9.0f; // 1.0f;
        float viewport_height = 2.0f;
        float viewport_width = aspect_ratio * viewport_height;
        float focal_length = 1.0f;

        origin = Point3(0.0f, 0.0f, 0.0f);
        horizontal = Vec3(viewport_width, 0.0f, 0.0f);
        vertical = Vec3(0.0f, viewport_height, 0.0f);
        // This assigment starts scanning from top left, but it will be flipped later
        lower_left = origin - horizontal / 2.0f - vertical / 2.0f - Vec3(0.0f, 0.0f, focal_length);
    }

      Ray get_ray(float u, float v) const { return Ray(origin, lower_left + u * horizontal + v * vertical - origin);
      }

      private:
          Point3 origin;
          Vec3 horizontal;
          Vec3 vertical;
          Point3 lower_left;


};

} // namespace sw
