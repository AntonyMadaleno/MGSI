/**
 * @file Vec.h
 * @brief Header for struct Vec2, Vec3 and Vec4
 * @author Antony Madaleno
 * @version 1.0
 * @date 12-01-2023
 *
 * Header pour les struct Vec2, Vec3, Vec4
 *
 */

#pragma once

/**
 * @struct Vec2
 * @brief Vector containing 2 floats x,y
 */
typedef struct Vec2
{
    float x, y;
} Vec2;

/**
 * @struct Vec3
 * @brief Vector containing 3 floats x,y,z
 */
typedef struct Vec3
{
    float x, y, z;
} Vec3;

/**
 * @struct Vec4
 * @brief Vector containing 4 floats x,y,z,w
 */
typedef struct Vec4
{
    float x, y, z, w;
} Vec4;

/**
 * @fn unsigned char Vec2_set(Vec2 * v, float x, float y)
 * @brief Function to set value of given 2D vector to given x,y
 *
 * @param v Pointer to vector
 * @param x Float value used to set x value inside vector v
 * @param y Float value used to set y value inside vector v
 * @return 0 if v is nullptr, 1 if values are set correctly
 */
unsigned char Vec2_set(Vec2 * v, float x, float y);

/**
 * @fn unsigned char Vec3_set(Vec3 * v, float x, float y, float z)
 * @brief Function to set value of given 3D vector to given x,y,z
 *
 * @param v Pointer to vector
 * @param x Float value used to set x value inside vector v
 * @param y Float value used to set y value inside vector v
 * @param z Float value used to set z value inside vector v
 * @return 0 if v is nullptr, 1 if values are set correctly
 */
unsigned char Vec3_set(Vec3 * v, float x, float y, float z);

/**
 * @fn unsigned char Vec4_set(Vec4 * v, float x, float y, float z, float w)
 * @brief Function to set value of given 4D vector to given x,y,z
 *
 * @param v Pointer to vector
 * @param x Float value used to set x value inside vector v
 * @param y Float value used to set y value inside vector v
 * @param z Float value used to set z value inside vector v
 * @param w Float value used to set w value inside vector v
 * @return 0 if v is nullptr, 1 if values are set correctly
 */
unsigned char Vec4_set(Vec4 * v, float x, float y, float z, float w);

/**
 * @fn float Vec2_length(Vec2 * v);
 * @brief Function to calculate length of said vector
 *
 * @param v Pointer to vector
 * @return length of the vector as float if v is nullptr return -1
 */
float Vec2_length(Vec2 * v);
/**
 * @fn float Vec3_length(Vec3 * v);
 * @brief Function to calculate length of said vector
 *
 * @param v Pointer to vector
 * @return length of the vector as float if v is nullptr return -1
 */
float Vec3_length(Vec3 * v);
/**
 * @fn float Vec4_length(Vec4 * v);
 * @brief Function to calculate length of said vector
 *
 * @param v Pointer to vector
 * @return length of the vector as float if v is nullptr return -1
 */
float Vec4_length(Vec4 * v);

/**
 * @fn float Vec2_length2(Vec2 * v);
 * @brief Function to calculate square length of said vector
 *
 * @param v Pointer to vector
 * @return length of the vector as float if v is nullptr return -1
 */
float Vec2_length2(Vec2 * v);
/**
 * @fn float Vec3_length2(Vec3 * v);
 * @brief Function to calculate square length of said vector
 *
 * @param v Pointer to vector
 * @return length of the vector as float if v is nullptr return -1
 */
float Vec3_length2(Vec3 * v);
/**
 * @fn float Vec4_length2(Vec4 * v);
 * @brief Function to calculate square length of said vector
 *
 * @param v Pointer to vector
 * @return length of the vector as float if v is nullptr return -1
 */
float Vec4_length2(Vec4 * v);

/**
 * @fn float Vec2_dist(Vec2 * v, Vec2 * u);
 * @brief Function to calculate distance in between 2 vertices
 *
 * @param v Pointer to vector
 * @param u Pointer to vector
 * @return distance in between given vertices as float, if v or u is nullptr return -1
 */
float Vec2_dist(Vec2 * v, Vec2 * u);
/**
 * @fn float Vec3_dist(Vec3 * v, Vec3 * u);
 * @brief Function to calculate distance in between 2 vertices
 *
 * @param v Pointer to vector
 * @param u Pointer to vector
 * @return distance in between given vertices as float, if v or u is nullptr return -1
 */
float Vec3_dist(Vec3 * v, Vec3 * u);\
/**
 * @fn float Vec4_dist(Vec4 * v, Vec4 * u);
 * @brief Function to calculate distance in between 2 vertices
 *
 * @param v Pointer to vector
 * @param u Pointer to vector
 * @return distance in between given vertices as float, if v or u is nullptr return -1
 */
float Vec4_dist(Vec4 * v, Vec4 * u);

/**
 * @fn float Vec2_dist2(Vec2 * v, Vec2 * u);
 * @brief Function to calculate distance in between 2 vertices
 *
 * @param v Pointer to vector
 * @param u Pointer to vector
 * @return square distance in between given vertices as float, if v or u is nullptr return -1
 */
float Vec2_dist2(Vec2 * v, Vec2 * u);
/**
 * @fn float Vec3_dist2(Vec3 * v, Vec3 * u);
 * @brief Function to calculate distance in between 2 vertices
 *
 * @param v Pointer to vector
 * @param u Pointer to vector
 * @return square distance in between given vertices as float, if v or u is nullptr return -1
 */
float Vec3_dist2(Vec3 * v, Vec3 * u);
/**
 * @fn float Vec4_dist2(Vec4 * v, Vec4 * u);
 * @brief Function to calculate distance in between 2 vertices
 *
 * @param v Pointer to vector
 * @param u Pointer to vector
 * @return square distance in between given vertices as float, if v or u is nullptr return -1
 */
float Vec4_dist2(Vec4 * v, Vec4 * u);

/// @fn unsigned char Vec2_to_Vec3(Vec3 * dest, Vec2 * src);
/// @brief transform given Vec2 to Vec3 by setting z to 0 into a Vec4 pointer
/// @param dest Vec3 to receipt data
/// @param src Vec2 source to transform
/// @return 0 if src is nullptr else 1
unsigned char Vec2_to_Vec3(Vec3 * dest, Vec2 * src);

/// @fn unsigned char Vec2_to_Vec4(Vec4 * dest, Vec2 * src);
/// @brief transform given Vec2 to Vec4 by setting z,w to 0 into a Vec4 pointer
/// @param dest Vec4 to receipt data
/// @param src Vec2 source to transform
/// @return 0 if src is nullptr else 1
unsigned char Vec2_to_Vec4(Vec4 * dest, Vec2 * src);

/// @fn unsigned char Vec3_to_Vec4(Vec4 * dest, Vec3 * src);
/// @brief transform given Vec3 to Vec4 by setting w to 0 into a Vec4 pointer
/// @param dest Vec4 to receipt data
/// @param src Vec3 source to transform
/// @return 0 if src is nullptr else 1
unsigned char Vec3_to_Vec4(Vec4 * dest, Vec3 * src);

/// @fn float Vec2_dot(Vec2 * v, Vec2 * u);
/// @brief return dot value of two vectors
/// @param v pointer Vec2
/// @param u pointer Vec2
/// @return dot value of two vectors as a float, If one of the 2 vectors is nullptr return 0
float Vec2_dot(Vec2 * v, Vec2 * u);

/// @fn float Vec2_dot(Vec3 * v, Vec3 * u);
/// @brief return dot value of two vectors
/// @param v pointer Vec3
/// @param u pointer Vec3
/// @return dot value of two vectors as a float, If one of the 2 vectors is nullptr return 0
float Vec3_dot(Vec3 * v, Vec3 * u);

/// @fn float Vec2_dot(Vec4 * v, Vec4 * u);
/// @brief return dot value of two vectors
/// @param v pointer Vec4
/// @param u pointer Vec4
/// @return dot value of two vectors as a float, If one of the 2 vectors is nullptr return 0
float Vec4_dot(Vec4 * v, Vec4 * u);

/// @fn Vec2_normalize(Vec2 * v);
/// @brief normalize given vector
/// @param v pointer to Vec2
/// @return 0 if v is nullptr else if length of v is not 0 return 1 else return 2 as unsigned char
unsigned char Vec2_normalize(Vec2 * v);

/// @fn Vec3_normalize(Vec3 * v);
/// @brief normalize given vector
/// @param v pointer to Vec3
/// @return 0 if v is nullptr else if length of v is not 0 return 1 else return 2 as unsigned char
unsigned char Vec3_normalize(Vec3 * v);

/// @fn Vec4_normalize(Vec4 * v);
/// @brief normalize given vector
/// @param v pointer to Vec4
/// @return 0 if v is nullptr else if length of v is not 0 return 1 else return 2 as unsigned char
unsigned char Vec4_normalize(Vec4 * v);

/// @fn Vec3 * Vec3_cross(Vec3 * v, Vec3 * u);
/// @brief function that calculate cross product of vectors v and u
/// @param v pointer to Vec3
/// @param u pointer to Vec3
/// @return pointer to Vec3 that is the cross product of v,u
Vec3 * Vec3_cross(Vec3 * v, Vec3 * u);

/// @fn void Vec2_print(Vec2 * v);
/// @brief print info about vector in console
/// @param v pointer to vector
void Vec2_print(Vec2 * v);

/// @fn void Vec2_print(Vec2 * v);
/// @brief print info about vector in console
/// @param v pointer to vector
void Vec3_print(Vec3 * v);

/// @fn void Vec2_print(Vec2 * v);
/// @brief print info about vector in console
/// @param v pointer to vector
void Vec4_print(Vec4 * v);

