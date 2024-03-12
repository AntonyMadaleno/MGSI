/**
 * @file Cylinder.h
 * @brief Header for struct Cylinder
 * @author Antony Madaleno
 * @version 1.0
 * @date 20-09-2023
 *
 * Header pour les struct Cylinders
 *
 */

#pragma once

#include "Vec.h"
#include "Quaternion.h"
#include "Object.h"

typedef struct Cylinder
{
    float h;
    float r;
} Cylinder;

/**
 * @brief 
 * 
 * @param h     taille en hauteur du cylindre
 * @param r     radius of the cylinder
 * @return Cylinder* 
 */
Cylinder * Cylinder_init(float h, float r);

/**
 * @brief 
 * 
 * @param Cylinder      pointer to cylinder struct
 * @param meridian      number of meridian (definition of the cylinder)
 * @param angle         portion of cylinder to draw (0 or 2*PI)
 * @param color
 * @param specular     
 * @param reflection 
 * @param shininess 
 * @return Object* 
 */
Object * Cylinder_generateSurface (

    Cylinder * Cylinder,
    unsigned short meridian,
    float angle,
    Vec3 * color,
    Vec3 * specular,
    float reflection,
    float shininess

);

