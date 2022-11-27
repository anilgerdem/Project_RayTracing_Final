#pragma once

#include "swVec3.h"
// Updated according to Ray Tracing in One Weekend,  Vec3 >> Point3 for points.
namespace sw {

class Ray {
  public:
    Ray() = default;
    Ray(const Point3 &o, const Vec3 &d, float t0 = 0.0f, float t1 = FLT_MAX) : orig(o), dir(d), minT(t0), maxT(t1) {}

    Point3 origin() const { return orig; }
    Vec3 direction() const { return dir; }

    Point3 point_at(float t) const { return orig + t * dir; }

  public:
    Point3 orig; 
    Vec3 dir;
    float minT{0.0f};
    float maxT{FLT_MAX};
};

} // namespace sw
