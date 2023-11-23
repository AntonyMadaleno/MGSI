/**
 * @file Surface.h
 * @brief Header for struct Surface
 * @author Antony Madaleno
 * @version 1.0
 * @date 09-11-2023
 *
 * Header pour les struct Surface
 *
 */

#pragma once

#include "Curve.h"
#include "Vec.h"
#include "Quaternion.h"
#include "Object.h"


/**
 * @struct Surface3D
 */
typedef struct Surface3D
{
    float * data;
    Vec3 ** Tu;
    Vec3 ** Tv;
    Vec3 ** normals;
    unsigned short N, M; //the number of points evaluated on u, v
} Surface3D;

/**
 * @brief initialize a surface given the following paramaters
 * @param Vec3 ** control_points the controls points defining the surface
 * etc..
 */
Surface3D * Surface3D_init( 
    Vec3 ** control_points,
    const unsigned char u_control_count,
    const unsigned char v_control_count,
    const unsigned short u_count, 
    const unsigned short v_count, 
    const enum methode mode_u, 
    const enum methode mode_v
);

/**
 * @brief 
 * 
 * @param s 
 * @param color 
 * @param specular_color 
 * @param shininess 
 * @param reflection 
 * @return Object* 
 */
Object * Surface3D_obejctify(Surface3D * s, Vec3 * color, Vec3 * specular_color, float shininess, float reflection);