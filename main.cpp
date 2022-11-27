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

// In the guide, it is called Ray_color

Color traceRay(const Ray &r, Scene scene, int depth) {
    Color c, directColor, reflectedColor, refractedColor;
    if (depth < 0) return c;

    Intersection hit, shadow;
    if (!scene.intersect(r, hit)) return Color(0.0f, 0.0f, 0.0f); // Background color

    const Vec3 lightPos(0.0f, 30.0f, -5.0f);
    Vec3 lightDir = lightPos - hit.position;
    lightDir.normalize();

    directColor = Color(1.0f, 1.0f, 1.0f);

    c = directColor;

    return c;
}

Color ray_color(const Ray &r) {
    Vec3 normal_direction = r.direction().normalize();
    auto t = 0.5f * (normal_direction.y() + 1.0f);
    return (1.0 - t) * Color(1.0f, 1.0f, 1.0f) + t * Color(0.5f, 0.7f, 1.0f);

    //return r.direction().normalize();
}

int main() {

    // Defining image features
    const float aspect_ratio = 16.0f / 9.0f;
    const int imageWidth = 800;
    const int imageHeight = static_cast<int>(imageWidth/aspect_ratio);
    const int numChannels = 3;
    uint8_t *pixels = new uint8_t[imageWidth * imageHeight * numChannels];

    // Camera set up
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    float focal_length = 1.0f;

    Point3 origin = Point3(0.0f, 0.0f, 0.0f);
    Vec3 horizontal = Vec3(viewport_width, 0.0f, 0.0f);
    Vec3 vertical = Vec3(0.0f, viewport_height, 0.0f);
    Point3 lower_left = origin - horizontal / 2.0f - vertical / 2.0f - Vec3(0.0f, 0.0f, focal_length);


    // Ray trace pixels
    int depth = 3;
    std::cout << "Rendering... ";
    clock_t start = clock();

    // In the guide, it starts scanning  from lower left,
    // This will be fixed later
    for (int j = imageHeight - 1; j >= 0; --j) {
        for (int i = 0; i < imageWidth; ++i) {

            float u = float(i) / (imageWidth - 1);
            float v = float(j) / (imageHeight - 1);
            Vec3 ray_direction = lower_left + u * horizontal + v * vertical - origin;
            Ray r = Ray(origin, ray_direction);

            Color pixel_color = ray_color(r);

            // Write pixel value to image
            writeColor((j * imageWidth + i) * numChannels, pixel_color, pixels);
        }
    }

    // IMAGE SHOULD BE FLIPPED FROM UP TO DOWN LATER
    // Save image to file
    stbi_write_png("out.png", imageWidth, imageHeight, numChannels, pixels, imageWidth * numChannels);

    // Free allocated memory
    delete[] pixels;

    std::cout << "Done\n";
    std::cout << "Time: " << (float)(clock() - start) / CLOCKS_PER_SEC << " s" << std::endl;
}
