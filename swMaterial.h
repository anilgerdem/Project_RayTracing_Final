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


class Dielectric : public Material {
  public:
    Dielectric(float index_of_refraction) : ir(index_of_refraction) {}

    virtual bool scatter(const Ray& r_in, const hit_record& rec, Color& attenuation, Ray& scattered) const override {
        attenuation = Color(1.0f, 1.0f, 1.0f);
        float refraction_ratio = rec.front_face ? (1.0f / ir) : ir;

        Vec3 unit_direction = r_in.direction().normalize();

        float cos_theta = fmin(-unit_direction * rec.normal, 1.0f);
        float sin_theta = sqrt(1.0f - cos_theta * cos_theta);

        bool cannot_refract = refraction_ratio * sin_theta > 1.0f;
        Vec3 direction;

        if (cannot_refract){ // Below is for schlick's approximation, it can be commented out and then glass will be more clear
        //if (cannot_refract || reflectance(cos_theta, refraction_ratio) > uniform_random()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, refraction_ratio);
        }

        scattered = Ray(rec.p, direction);
        return true;
    }

    public:
    float ir;

    private:
        static float reflectance(float cosine, float ref_index) {
            // Schlick's approximation for reflectance , from the book
            float r0 = (1.0f - ref_index) / (1 + ref_index);
            r0 = r0 * r0;
            return r0 + (1.0 - r0) * pow((1.0f - cosine), 5.0f);
    }

};

} // namespace sw
