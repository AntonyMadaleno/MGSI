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
    float rx;
    float ry;
    float rz;
} Sphere;

/**
 * @brief 
 * 
 * @param rx 
 * @param ry 
 * @param rz 
 * @return Sphere* 
 */
Sphere * Sphere_init(float rx, float ry, float rz);

/**
 * @brief 
 * 
 * @param sphere 
 * @param meridian 
 * @param parallel 
 * @param color 
 * @param specular 
 * @param reflection 
 * @param shininess 
 * @return Object *
 */
Object * Sphere_generateSurface (

    Sphere * sphere,
    unsigned char meridian,
    unsigned char parallel,
    Vec3 * color,
    Vec3 * specular,
    float reflection,
    float shininess

);

