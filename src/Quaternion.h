/**
 * @file Quaternion.h
 * @brief Header for struct Quaternion
 * @author Antony Madaleno
 * @version 1.0
 * @date 12-09-2023
 *
 * Header pour les struct de Quaternions
 *
 */

#ifndef QUATERNION_H
#define QUATERNION_H

#include "Vec.h"

/** 
 * @struct Quaternion
 * @brief Represents a quaternion in the form q = w + xi + yj + zk.
 */
typedef struct {
    float w; /**< Real component of the quaternion. */
    float x; /**< Imaginary component (i) of the quaternion. */
    float y; /**< Imaginary component (j) of the quaternion. */
    float z; /**< Imaginary component (k) of the quaternion. */
} Quaternion;

/**
 * @brief Initializes a quaternion with the given values.
 * 
 * @param w Real component.
 * @param x Imaginary component (i).
 * @param y Imaginary component (j).
 * @param z Imaginary component (k).
 * @return Initialized quaternion.
 */
Quaternion * Quaternion_Init(float w, float x, float y, float z);

/**
 * @brief Computes the magnitude (norm) of a quaternion.
 * 
 * @param q The input quaternion.
 * @return Magnitude of the quaternion.
 */
float Quaternion_Magnitude(const Quaternion* q);

/**
 * @brief Normalizes a quaternion to have unit magnitude.
 * 
 * @param q The input quaternion.
 */
void Quaternion_Normalize(Quaternion* q);

/**
 * @brief Performs quaternion multiplication (Hamilton product) q1 * q2.
 * 
 * @param q1 The first quaternion.
 * @param q2 The second quaternion.
 * @return Result of the multiplication.
 */
Quaternion * Quaternion_Multiply(const Quaternion* q1, const Quaternion* q2);

/**
 * @brief Rotates a vector by a quaternion.
 * 
 * @param v The input vector.
 * @param q The quaternion representing the rotation.
 * @return Rotated vector.
 */
Vec3 * Quaternion_RotateVector(const Vec3* v, const Quaternion* q);

/**
 * @brief 
 * 
 * @param angleInDegrees 
 * @param axis 
 * @return Quaternion* 
 */
Quaternion* Quaternion_fromAxisAngle(float angleInDegrees, Vec3* axis);

#endif /* QUATERNION_H */