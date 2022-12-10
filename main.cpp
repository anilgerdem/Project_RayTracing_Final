/*
 *  main.cpp
 *  swTracer
 *
 *  Created by Michael Doggett on 2021-09-23.
 *  Copyright (c) 2021 Michael Doggett
 */

/*
Combining EDAN35 High Performance Computer Graphics Assignment 1with Raytracing in One Weekend for the course project

Title (series): “Ray Tracing in One Weekend Series”
Title (book): “Ray Tracing in One Weekend”
Author: Peter Shirley Editors: Steve Hollasch, Trevor David Black
Version/Edition: v3.2.3 Date: 2020-12-07 URL (series):
https://raytracing.github.io/ URL (book): https://raytracing.github.io/books/RayTracingInOneWeekend.html 

*/
#define _USE_MATH_DEFINES
#include <cfloat>
#include <cmath>
#include <ctime>
#include <iostream>
#include <random>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "swCamera.h"
#include "swIntersection.h"
#include "swMaterial.h"
#include "swRay.h"
#include "swScene.h"
#include "swSphere.h"
#include "swVec3.h"
#include "swCommons.h"
#include "swHittable_list.h"

using namespace sw;

inline float clamp(float x, float min, float max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

float uniform() {
    // Will be used to obtain a seed for the random number engine
    static std::random_device rd;
    // Standard mersenne_twister_engine seeded with rd()
    static std::mt19937 gen(rd());
    static std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

void writeColor(int index, Vec3 p, uint8_t *pixels) {
    // gamma correct for gamma=2.2, x^(1/gamma), more see :
    // https://www.geeks3d.com/20101001/tutorial-gamma-correction-a-story-of-linearity/
    for (int n = 0; n < 3; n++) {
        p.m[n] = pow(p.m[n], 1.0f / 2.2f);
        pixels[index + n] = (uint8_t)(256 * clamp(p.m[n], 0.0f, 0.999f));
    }
}

// In the book, it is called Ray_color

/*
// Combination of assigment sphere:intersection and the book
float hit_sphere(const Point3 &center, float radius, const Ray &r) {
    Vec3 o = r.orig - center;
    Vec3 d = r.dir;

    // Compute polynom coefficients.
    float A = d * d;
    float B = 2.0f * d * o;
    float C = o * o - radius * radius;

    float discri = B * B - 4.0f * A * C;
    if (discri < 0.0f) {
        return -1.0f;
    } else {
        return (-B - std::sqrt(discri)) / (2.0f * A);
    }
}
*/


Color ray_color(const Ray &r, Hittable &world, int depth) {

    hit_record rec;

    if (depth <= 0) return Color(0.0f, 0.0f, 0.0f);

    if (world.hit(r, 0.001f, FLT_MAX, rec)) {
        //Point3 target = rec.p + rec.normal + random_unit_vector();
        ////return  0.5 * (rec.normal + 1.0f); // Mapping points from -1:1 to 0:1 , Vec3 and float addition is defined  // for testing Color( 1.0f, 1.0f, 1.0f); //
        //return 0.5 * ray_color(Ray(rec.p, target - rec.p), world, depth - 1);

        Ray scattered;
        Color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered)) {
            Color col = ray_color(scattered, world, depth - 1);
            return Color(attenuation.x() * col.x(), attenuation.y() * col.y(), attenuation.z() * col.z());
        }
        return Color(0.0f, 0.0f, 0.0f);
            

    } else {
        Vec3 normal_direction = r.direction().normalize();
        float t = 0.5f * (normal_direction.y() + 1.0f);
        return (1.0 - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);
    }
    }




int main() {

    // Defining image features
    const float aspect_ratio = 16.0f / 9.0f; // 1.0f;
    const int imageWidth = 800;
    const int imageHeight = static_cast<int>(imageWidth/aspect_ratio);
    const int numChannels = 3;
    uint8_t *pixels = new uint8_t[imageWidth * imageHeight * numChannels];

    const int super_sampling = 5;
    const int max_depth = 1000;

    // Camera
    Camera camera;

    // Creating the scene
    Hittable_list world;
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.7, 0.3, 0.3));
    auto material_left = make_shared<Metal>(Color(0.8, 0.8, 0.8), 0.3f);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0f);

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.0), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

    // Ray trace pixels
    int depth = 3;
    std::cout << "Rendering... ";
    clock_t start = clock();


    // In the guide, it starts scanning  from lower left,
    // This will be fixed later  //int j = imageHeight - 1; j >= 0; --j   //int j = 0; j < imageHeight; ++j
    for (int j = imageHeight - 1; j >= 0; --j) {
        for (int i = 0; i < imageWidth; ++i) {

            Color pixel_color = Color(0.0f, 0.0f, 0.0f);

            //// SUPER SAMPLING
            Color pixel_color_ss = Color(0.0f, 0.0f, 0.0f);
            for (int ss=0; ss<super_sampling; ++ss) {
                float u = float(i + uniform()) / (imageWidth - 1);
                float v = float(j + uniform()) / (imageHeight - 1);
                Ray r = camera.get_ray(u,v);
                pixel_color_ss = pixel_color_ss + ray_color(r, world, max_depth);
            }
            pixel_color = pixel_color_ss / super_sampling;
            //// SUPER SAMPLING END


            // NORMAL SAMPLING
            //float u = float(i) / (imageWidth - 1);
            //float v = float(j) / (imageHeight - 1);
            //Ray r = camera.get_ray(u, v);
            //pixel_color = ray_color(r, world, max_depth);
            // NORMAL SAMPLING END

            // Write pixel value to image
            writeColor((j * imageWidth + i) * numChannels, pixel_color, pixels);
            
        }
    }

    // IMAGE IS FLIPPED, IT WILL BE FIXED LATER

    // Save image to file
    stbi_write_png("out.png", imageWidth, imageHeight, numChannels, pixels, imageWidth * numChannels);

    // Free allocated memory
    delete[] pixels;

    std::cout << "Done\n";
    std::cout << "Time: " << (float)(clock() - start) / CLOCKS_PER_SEC << " s" << std::endl;
}
