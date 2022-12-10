#pragma once

#include "swVec3.h"
#include "swHittable.h"

using std::shared_ptr;
using std::make_shared;

namespace sw {

struct hit_record;

class Material {
  public:
  //  Material() = default;
  //  // color, reflectivity, transparency, refractiveIndex
  //  Material(const Vec3 &c, float r = 0, float t = 0, float i = 1)
  //    : color{c}, reflectivity(r), transparency(t), refractiveIndex(i) {}

  //public:
  //  Vec3 color;
  //  float reflectivity{0.0f};
  //  float transparency{0.0f};
  //  float refractiveIndex{1.0f};

    Material() = default;
    virtual bool scatter(const Ray &r_in, const hit_record &rec, Color &attenuation, Ray &scattered) const = 0;
};

class Lambertian : public Material {
  public:
    Lambertian(const Color &a) : albedo(a) {}

    virtual bool scatter(const Ray &r_in, const hit_record &rec, Color &attenuation, Ray &scattered) const override {

        Vec3 scatter_direction = rec.normal + random_unit_vector();

        if (scatter_direction.near_zero()) scatter_direction = rec.normal; 

        scattered = Ray(rec.p, scatter_direction);
        attenuation = albedo;
        return true;
    }

    public:
    Color albedo;
};

class Metal : public Material {
  public:
    Metal(const Color &a, float f) : albedo(a), fuzz(f < 1.0f ? f:1) {}
    virtual bool scatter(const Ray &r_in, const hit_record &rec, Color &attenuation, Ray &scattered) const override {
        Vec3 reflected = reflect(r_in.direction().normalize(), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz*random_in_unit_sphere());
        attenuation = albedo;

        return ((scattered.direction() * rec.normal) > 0);
    }

  public:
    Color albedo;
    float fuzz;
};

} // namespace sw
