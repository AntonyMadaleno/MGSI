#define _USE_MATH_DEFINES
#include <math.h>
#include "quaternion.h" // Include your quaternion header file here

// Initialize a quaternion with the given values
Quaternion * Quaternion_Init(float w, float x, float y, float z) {
    Quaternion * q = (Quaternion *) calloc( 1, sizeof(Quaternion) );
    q->w = w;
    q->x = x;
    q->y = y;
    q->z = z;
    return q;
}

// Compute the magnitude (norm) of a quaternion
float Quaternion_Magnitude(const Quaternion* q) {
    return sqrtf(q->w * q->w + q->x * q->x + q->y * q->y + q->z * q->z);
}

// Normalize a quaternion to have unit magnitude
void Quaternion_Normalize(Quaternion* q) {
    float magnitude = Quaternion_Magnitude(q);
    if (magnitude > 0.0f) {
        q->w /= magnitude;
        q->x /= magnitude;
        q->y /= magnitude;
        q->z /= magnitude;
    }
}

// Perform quaternion multiplication (Hamilton product) q1 * q2
Quaternion * Quaternion_Multiply(const Quaternion* q1, const Quaternion* q2) {
    Quaternion * result = (Quaternion *) calloc( 1, sizeof(Quaternion) );
    result->w = q1->w * q2->w - q1->x * q2->x - q1->y * q2->y - q1->z * q2->z;
    result->x = q1->w * q2->x + q1->x * q2->w + q1->y * q2->z - q1->z * q2->y;
    result->y = q1->w * q2->y - q1->x * q2->z + q1->y * q2->w + q1->z * q2->x;
    result->z = q1->w * q2->z + q1->x * q2->y - q1->y * q2->x + q1->z * q2->w;
    return result;
}

Quaternion* Quaternion_fromAxisAngle(float angleInDegrees, Vec3* axis) {
    float angleInRadians = angleInDegrees * M_PI / 180.0;
    float halfAngle = angleInRadians / 2.0;
    float sinHalfAngle = sin(halfAngle);
    Quaternion* q = Quaternion_Init(cos(halfAngle), axis->x * sinHalfAngle, axis->y * sinHalfAngle, axis->z * sinHalfAngle);
    return q;
}

// Rotate a vector by a quaternion
Vec3 * Quaternion_RotateVector(const Vec3* v, const Quaternion* q) {
    Quaternion * vQuat = Quaternion_Init(0.0f, v->x, v->y, v->z);
    Quaternion * conjugate = Quaternion_Init(q->w, -q->x, -q->y, -q->z);
    Quaternion * rotatedVector = Quaternion_Multiply(q, vQuat);
    Quaternion * rV = Quaternion_Multiply(rotatedVector, conjugate);
    Vec3 * result = (Vec3 *) calloc( 1, sizeof(Vec3) );
    Vec3_set(result, rV->x, rV->y, rV->z);

    free(vQuat);
    free(conjugate);
    free(rotatedVector);
    free(rV);

    return result;
}