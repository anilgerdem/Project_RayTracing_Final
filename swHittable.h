#pragma once

#include "swRay.h"
namespace sw {

struct hit_record {
    float t;
    Point3 p;
    Vec3 normal;

    bool front_face;

    inline void set_face_normal(const Ray &r, const Vec3 &outward_normal) {
        front_face = (r.direction() * outward_normal < 0.0f);
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable {
  public:
    virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};

} // namespace sw
