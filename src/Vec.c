/**
 * @file Vec.c
 * @brief Implement Vec.h
 * @author Antony Madaleno
 * @version 1.0
 * @date 12-01-2023
 */

#include <stdlib.h>
#include <stdio.h>
#include "math.h"
#include "Vec.h"

unsigned char Vec2_set(Vec2 * v, float x, float y)
{
    if (v == NULL)
        return 0;
    
    v->x = x;
    v->y = y;

    return 1;

}

unsigned char Vec3_set(Vec3 * v, float x, float y, float z)
{
    if (v == NULL)
        return 0;
    
    v->x = x;
    v->y = y;
    v->z = z;

    return 1;

}

unsigned char Vec4_set(Vec4 * v, float x, float y, float z, float w)
{
    if (v == NULL)
        return 0;
    
    v->x = x;
    v->y = y;
    v->z = z;
    v->w = w;

    return 1;
}

float Vec2_length(Vec2 * v)
{
    if (v == NULL)
        return -1;

    return sqrtf(v->x * v->x + v->y * v->y);
}

float Vec3_length(Vec3 * v)
{
    if (v == NULL)
        return -1;

    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z);
}

float Vec4_length(Vec4 * v)
{
    if (v == NULL)
        return -1;

    return sqrtf(v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w);
}

float Vec2_length2(Vec2 * v)
{
    if (v == NULL)
        return -1;

    return v->x * v->x + v->y * v->y;
}

float Vec3_length2(Vec3 * v)
{
    if (v == NULL)
        return -1;

    return v->x * v->x + v->y * v->y + v->z * v->z;
}

float Vec4_length2(Vec4 * v)
{
    if (v == NULL)
        return -1;

    return v->x * v->x + v->y * v->y + v->z * v->z + v->w * v->w;
}

float Vec2_dist(Vec2 * v, Vec2 * u)
{
    if (v == NULL || u == NULL)
        return -1;

    float dist;
    Vec2 * tmp = (Vec2 *) calloc( 1, sizeof(Vec2) );

    Vec2_set(tmp, v->x - u->x, v->y - u->y);
    dist = Vec2_length(tmp);

    free(tmp);
    return dist;
}

float Vec3_dist(Vec3 * v, Vec3 * u)
{
    if (v == NULL || u == NULL)
        return -1;

    float dist;
    Vec3 * tmp = (Vec3 *) calloc( 1, sizeof(Vec3) );

    Vec3_set(tmp, v->x - u->x, v->y - u->y, v->z - u->z);
    dist = Vec3_length(tmp);

    free(tmp);
    return dist;
}

float Vec4_dist(Vec4 * v, Vec4 * u)
{
    if (v == NULL || u == NULL)
        return -1;

    float dist;
    Vec4 * tmp = (Vec4 *) calloc( 1, sizeof(Vec4) );

    Vec4_set(tmp, v->x - u->x, v->y - u->y, v->z - u->z, v->w - u->w);
    dist = Vec4_length(tmp);

    free(tmp);
    return dist;
}

float Vec2_dist2(Vec2 * v, Vec2 * u)
{
    if (v == NULL || u == NULL)
        return -1;

    float dist;
    Vec2 * tmp = (Vec2 *) calloc( 1, sizeof(Vec2) );

    Vec2_set(tmp, v->x - u->x, v->y - u->y);
    dist = Vec2_length2(tmp);

    free(tmp);
    return dist;
}

float Vec3_dist2(Vec3 * v, Vec3 * u)
{
    if (v == NULL || u == NULL)
        return -1;

    float dist;
    Vec3 * tmp = (Vec3 *) calloc( 1, sizeof(Vec3) );

    Vec3_set(tmp, v->x - u->x, v->y - u->y, v->z - u->z);
    dist = Vec3_length2(tmp);

    free(tmp);
    return dist;
}

float Vec4_dist2(Vec4 * v, Vec4 * u)
{
    if (v == NULL || u == NULL)
        return -1;

    float dist;
    Vec4 * tmp = (Vec4 *) calloc( 1, sizeof(Vec4) );

    Vec4_set(tmp, v->x - u->x, v->y - u->y, v->z - u->z, v->w - u->w);
    dist = Vec4_length2(tmp);

    free(tmp);
    return dist;
}

unsigned char Vec2_to_Vec3(Vec3 * dest, Vec2 * src)
{
    if (src == NULL)
        return 0;

    if (dest == NULL)
        dest = (Vec3 *) calloc( 1, sizeof(Vec3) );

    Vec3_set(dest, src->x, src->y, 0);

    return 1;
}

unsigned char Vec2_to_Vec4(Vec4 * dest, Vec2 * src)
{
    if (src == NULL)
        return 0;

    if (dest == NULL)
        dest = (Vec4 *) calloc( 1, sizeof(Vec4) );

    Vec4_set(dest, src->x, src->y, 0, 0);

    return 1;
}

unsigned char Vec3_to_Vec4(Vec4 * dest, Vec3 * src)
{
    if (src == NULL)
        return 0;

    if (dest == NULL)
        dest = (Vec4 *) calloc( 1, sizeof(Vec4) );

    Vec4_set(dest, src->x, src->y, src->z, 0);

    return 1;
}

float Vec2_dot(Vec2 * v, Vec2 * u)
{
    if (v == NULL || u == NULL)
        return 0;
    
    return v->x * u->x + v->y * u->y;
}

float Vec3_dot(Vec3 * v, Vec3 * u)
{
    if (v == NULL || u == NULL)
        return 0;

    return v->x * u->x + v->y * u->y + v->z * u->z;
}

float Vec4_dot(Vec4 * v, Vec4 * u)
{
    if (v == NULL || u == NULL)
        return 0;

    return v->x * u->x + v->y * u->y + v->z * u->z + v->w * u->x;
}

unsigned char Vec2_normalize(Vec2 * v)
{
    if (v == NULL)
        return 0;

    float len = Vec2_length(v);

    if (len == 0)
        return 2;

    if (len != 1)
    {
        v->x /= len;
        v->y /= len;
    }
    
    return 1;
}

unsigned char Vec3_normalize(Vec3 * v)
{
    if (v == NULL)
        return 0;

    float len = Vec3_length(v);

    if (len == 0)
        return 2;

    if (len != 1)
    {
        v->x /= len;
        v->y /= len;
        v->z /= len;
    }
    
    return 1;
}

unsigned char Vec4_normalize(Vec4 * v)
{
    if (v == NULL)
        return 0;

    float len = Vec4_length(v);

    if (len == 0)
        return 2;

    if (len != 1)
    {
        v->x /= len;
        v->y /= len;
        v->z /= len;
        v->w /= len;
    }
    
    return 1;
}

Vec3 * Vec3_cross(Vec3 * v, Vec3 * u)
{
    if (v == NULL || u == NULL)
    {
        printf("One of the pointers is NULL !\n");
        exit(-1);
    }

    Vec3 * n = (Vec3 *) calloc( 1, sizeof(Vec3) );

    n->x = v->y * u->z - v->z * u->y;
    n->y = v->z * u->x - v->x * u->z;
    n->z = v->x * u->y - v->y * u->x;

    return n;
}

void Vec2_print(Vec2 * v)
{
    printf("(%3.2f,\t%3.2f)\n", v->x, v->y);
}

void Vec3_print(Vec3 * v)
{
    printf("(%3.2f,\t%3.2f,\t%3.2f)\n", v->x, v->y, v->z);
}

void Vec4_print(Vec4 * v)
{
    printf("(%3.2f,\t%3.2f,\t%3.2f,\t%3.2f)\n", v->x, v->y, v->z, v->w);
}