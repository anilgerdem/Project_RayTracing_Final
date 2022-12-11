#pragma once

#define _USE_MATH_DEFINES
#include <cfloat>
#include <cmath>
#include <iostream>
#include <random>

using std::sqrt;

namespace sw {

class Vec3 {
  public:
    Vec3() = default;

    Vec3(float m0, float m1, float m2) : m{m0, m1, m2} {}

    float x() const { return m[0]; }
    float y() const { return m[1]; }
    float z() const { return m[2]; }

    // CODE: To return RGB, same as xyz
    float r() const { return m[0]; }
    float g() const { return m[1]; }
    float b() const { return m[2]; }
    // CODE END

    float operator[](int i) const { return m[i]; } //

    Vec3 operator*(int a) const { return Vec3((float)a * m[0], (float)a * m[1], (float)a * m[2]); }

    Vec3 operator*(float a) const { return Vec3(a * m[0], a * m[1], a * m[2]); }

    friend Vec3 operator*(float a, const Vec3 &v) { return Vec3(a * v[0], a * v[1], a * v[2]); }

    Vec3 operator+(const Vec3 &v) const { return Vec3(m[0] + v[0], m[1] + v[1], m[2] + v[2]); }

    Vec3 operator-() const { return Vec3(-m[0], -m[1], -m[2]); } //

    Vec3 operator-(const Vec3 &v) const { return Vec3(m[0] - v[0], m[1] - v[1], m[2] - v[2]); }

    float operator*(const Vec3 &v) const { return m[0] * v[0] + m[1] * v[1] + m[2] * v[2]; }

    Vec3 operator%(const Vec3 &v) const {
        return Vec3(m[1] * v[2] - m[2] * v[1], m[2] * v[0] - m[0] * v[2], m[0] * v[1] - m[1] * v[0]);
    }

    Vec3 &operator+=(const Vec3 &v) {
        m[0] += v[0];
        m[1] += v[1];
        m[2] += v[2];
        return *this;
    }

    Vec3 &operator*=(float a) {
        m[0] *= a;
        m[1] *= a;
        m[2] *= a;
        return *this;
    }

    Vec3 &normalize() {
        float l = 1.0f / std::sqrt(m[0] * m[0] + m[1] * m[1] + m[2] * m[2]);
        m[0] *= l;
        m[1] *= l;
        m[2] *= l;
        return *this;
    }

    // CODE: Ray Tracing in one Weekend

    Vec3 operator+() const { return Vec3(m[0], m[1], m[2]); } // for consistency; + operator, - is defined above
    Vec3 operator+(float a) const { return Vec3(a + m[0], a + m[1], a + m[2]); } // good to have Vec3 + float function
    Vec3 operator/(float a) const { return Vec3(m[0] / a, m[1] / a, m[2] / a); }

    //Vec3 &operator+=(const Vec3 &v); //ALREADY DEFINED

    Vec3 &operator-=(const Vec3 &v) {
        m[0] -= v[0];
        m[1] -= v[1];
        m[2] -= v[2];
        return *this;
    }
    Vec3 &operator*=(const Vec3 &v) {
        m[0] *= v[0];
        m[1] *= v[1];
        m[2] *= v[2];
        return *this;
    }
    //Vec3 &operator/=(const Vec3 &v);
    //Vec3 &operator*=(const float a); //ALREADY DEFINED
    Vec3 &operator/=(const float a) {
        m[0] /= a;
        m[1] /= a;
        m[2] /= a;
        return *this;
    }

    float length() const { return sqrt(length_squared()); }

    float length_squared() const { return m[0] * m[0] + m[1] * m[1] + m[2] * m[2]; }

    bool near_zero() const {
        const auto s = 1e-8;
        return (fabs(m[0]) < s) && (fabs(m[1]) < s) && (fabs(m[2]) < s); 
    }

    //CODE END 

  public:
    float m[3]{0.0f, 0.0f, 0.0f};
};

using Color = Vec3; // RGB color
using Point3 = Vec3; // 3D point

float uniform_random() {
    // Will be used to obtain a seed for the random number engine
    static std::random_device rd;
    // Standard mersenne_twister_engine seeded with rd()
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

Vec3 random_in_unit_sphere() {
    while (true) {
        float ran1 = uniform_random() * 2.0f - 1.0f;
        float ran2 = uniform_random() * 2.0f - 1.0f;
        float ran3 = uniform_random() * 2.0f - 1.0f;

        Vec3 point = Vec3(ran1, ran2, ran3);

        if (point.length_squared() >= 1.0f) continue;
        return point;
    }

}

Vec3 random_unit_vector() { return random_in_unit_sphere().normalize(); }

Vec3 reflect(const Vec3 &v, const Vec3 &n) { return v - 2.0f * (v * n) * n; }


Vec3 refract(const Vec3 &uv, const Vec3 &n, float etai_over_etat) {
    float cos_theta = std::fmin((-uv * n), 1.0f);
    Vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vec3 r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.length_squared())) * n;

    return r_out_perp + r_out_parallel;
}

} // namespace sw
