#pragma once

#include "swRay.h"
#include "swCommons.h"

namespace sw {

class Camera {
  public:
    Camera() = default;
    Camera(Point3 lookFrom, Point3 lookAt, Vec3 vup, float vfov, float aspect_ratio) {
        // Camera set up
        float theta = degrees_to_radians(vfov);
        float h = tan(theta / 2.0f);
        float viewport_height = 2.0f * h;
        float viewport_width = aspect_ratio * viewport_height;

        Vec3 w = (lookFrom - lookAt).normalize();
        Vec3 u = (vup % w).normalize();
        Vec3 v = w % u;

        float focal_length = 1.0f;

        origin = lookFrom;
        horizontal = viewport_width * u;
        vertical = viewport_height * v;
        // This assigment starts scanning from top left, but it will be flipped later
        lower_left = origin - horizontal / 2.0f - vertical / 2.0f - w;
    }

      Ray get_ray(float s, float t) const { return Ray(origin, lower_left + s * horizontal + t * vertical - origin);
      }

      private:
          Point3 origin;
          Vec3 horizontal;
          Vec3 vertical;
          Point3 lower_left;


};

} // namespace sw
