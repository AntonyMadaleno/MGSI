/**
 * @file Curve.h
 * @brief Header for struct Curve
 * @author Antony Madaleno
 * @version 1.0
 * @date 13-09-2023
 *
 * Header pour les struct Curve
 *
 */

#pragma once

#include "Vec.h"
#include "Quaternion.h"
#include "Object.h"

/**
 * @struct Curve3D
 */
typedef struct Curve3D
{
    float * data;
    Vec3 * T;
    Vec3 * N;
    Vec3 * B;
    unsigned short npoints; //the number of points evaluated
} Curve3D;

enum methode
{
    BEZIER,
    CATMULL_ROM,
    NUBS
};

/**
 * @brief 
 * 
 * @param control_points the control points/vectors that describe de curve
 * @param n_points the number of points to be generated on the curve
 * @param mode the methode used to generate the curve from the control points/vectors
 * @return Curve3D* pointer to the result
 */
Curve3D * Curve3D_init(Vec3* control_points, unsigned char n_control_points , const unsigned short n_points, const enum methode mode);

/**
 * @brief generate the TNB frame for every evaluated point on the curve /!\ curve must have been initialized
 */
void Curve3D_calculateTNB(Curve3D * c);

Object * Curve3D_generateSurface(Curve3D * c, unsigned char meridians, Vec3 * color, Vec3 * specular_color, float shininess, float reflectivness
, float radius);

