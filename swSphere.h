#pragma once

#include "swHittable.h"

namespace sw {

class Sphere : public Hittable {
  public:
    Sphere() = default;
    Sphere(const Vec3 &c, const float &r) : center(c), radius(r) {}
    Sphere(const Sphere &s) = default;
    Sphere(Sphere &&) = default;
    Sphere &operator=(Sphere &&) = default;

    virtual bool hit(const Ray &r, float t_min, float t_max, hit_record &rec) const;

  public:
    Vec3 center;
    float radius{0.0f};
};

bool Sphere::hit(const Ray &r, float t_min, float t_max, hit_record &rec) const {
    // Below calculations are from the assignment
    Vec3 o = r.orig - center;
    Vec3 d = r.dir;

    // Compute polynom coefficients.
    float A = d * d;
    float B = 2.0f * d * o;
    float C = o * o - radius * radius;

    float discri = B * B - 4.0f * A * C;
    if (discri < 0.0f) {
        return false;
    }

    // hit points
    float sqrt_discri = std::sqrt(discri);
    float t0 = (-B - sqrt_discri) / (2.0f * A);
    float t1 = (-B + sqrt_discri) / (2.0f * A);

    if (t0 > t_max || t1 < t_min) {
        return false;
    }
    if (t0 < t_min && t1 > t_max) {
        return false;
    }

    rec.t = t0 < t_min ? t1 : t0;
    rec.p = r.at(rec.t);
    Vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
};

} // namespace sw
