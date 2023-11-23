/**
 * @file Sphere.h
 * @brief Header for struct Sphere
 * @author Antony Madaleno
 * @version 1.0
 * @date 20-09-2023
 *
 * Header pour les struct Spheres
 *
 */

#pragma once

#include "Vec.h"
#include "Quaternion.h"
#include "Object.h"

typedef struct Sphere
{
    Vec3 * position;
    float rx;
    float ry;
    float rz;
} Sphere;


Sphere * Sphere_init(Vec3 * position, float rx, float ry, float rz);

Object * Sphere_generateSurface(
    Sphere * sphere,
    unsigned char meridian,
    unsigned char parallel,
    Vec3 * color,
    Vec3 * specular,
    float reflection,
    float shininess
);

