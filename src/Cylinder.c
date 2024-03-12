#include "Cylinder.h"
#include "pthread.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

Cylinder * Cylinder_init(float h, float r)
{
    Cylinder * c = (Cylinder *) calloc(1, sizeof(Cylinder) );
    c->r = r;
    c->h = h;

    return c;
}

Object * Cylinder_generateSurface(
    Cylinder * cylinder,
    unsigned short meridian,
    float angle,
    Vec3 * color,
    Vec3 * specular,
    float reflection,
    float shininess
)
{

    //CREATE THE LAYOUT
    unsigned char * layout = (unsigned char *) calloc(6, sizeof(unsigned char));
    layout[0] = 3; //Vertex Position
    layout[1] = 3; //Vertex Normal
    layout[2] = 3; //Vertex Color
    layout[3] = 3; //Vertex Specular
    layout[4] = 1; //Vertex Shininess
    layout[5] = 1; //Vertex Reflection

    unsigned char size = 14;

    //CREATE THE OBJECT
    Object * obj = Object_init( meridian * 2 , 6, layout, 2 * meridian );

    float alpha = 0.0;

    for (unsigned short i = 0; i < meridian; i++)
    {

        if (angle != 0.0)
            alpha = ((float) i) / ((float) meridian) * angle;
        else
            alpha = ((float) i) / ((float) meridian) * 2 * M_PI;

        //vertex bottom

        obj->vertexBuffer[2 * i * size + 0]  = cylinder->r * cosf( alpha );
        obj->vertexBuffer[2 * i * size + 1]  = 0.0f;
        obj->vertexBuffer[2 * i * size + 2]  = cylinder->r * sinf( alpha );

        obj->vertexBuffer[2 * i * size + 3]  = obj->vertexBuffer[2 * i * size + 0];
        obj->vertexBuffer[2 * i * size + 4]  = 0.0f;
        obj->vertexBuffer[2 * i * size + 5]  = obj->vertexBuffer[2 * i * size + 2];

        obj->vertexBuffer[2 * i * size + 6]  = color->x;
        obj->vertexBuffer[2 * i * size + 7]  = color->y;
        obj->vertexBuffer[2 * i * size + 8]  = color->z;

        obj->vertexBuffer[2 * i * size + 9]  = specular->x;
        obj->vertexBuffer[2 * i * size + 10] = specular->y;
        obj->vertexBuffer[2 * i * size + 11] = specular->z;

        obj->vertexBuffer[2 * i * size + 12] = shininess;
        obj->vertexBuffer[2 * i * size + 13] = reflection;

        //vertex upper

        obj->vertexBuffer[2 * i * size + 0 + size]  = cylinder->r * cosf( alpha );
        obj->vertexBuffer[2 * i * size + 1 + size]  = cylinder->h;
        obj->vertexBuffer[2 * i * size + 2 + size]  = cylinder->r * sinf( alpha );

        obj->vertexBuffer[2 * i * size + 3 + size]  = obj->vertexBuffer[2 * i * size + 0];
        obj->vertexBuffer[2 * i * size + 4 + size]  = 0.0f;
        obj->vertexBuffer[2 * i * size + 5 + size]  = obj->vertexBuffer[2 * i * size + 2];

        obj->vertexBuffer[2 * i * size + 6 + size]  = color->x;
        obj->vertexBuffer[2 * i * size + 7 + size]  = color->y;
        obj->vertexBuffer[2 * i * size + 8 + size]  = color->z;

        obj->vertexBuffer[2 * i * size + 9 + size]  = specular->x;
        obj->vertexBuffer[2 * i * size + 10 + size] = specular->y;
        obj->vertexBuffer[2 * i * size + 11 + size] = specular->z;

        obj->vertexBuffer[2 * i * size + 12 + size] = shininess;
        obj->vertexBuffer[2 * i * size + 13 + size] = reflection;

        //index
        unsigned in = (i+1) % meridian;

        unsigned p0 = 2 * i;
        unsigned p1 = 2 * in;
        unsigned p2 = 2 * i  + 1;
        unsigned p3 = 2 * in + 1;

        unsigned int index = i*6;

        obj->indexBuffer[ index + 0 ] = p0;
        obj->indexBuffer[ index + 1 ] = p2;
        obj->indexBuffer[ index + 2 ] = p3;

        obj->indexBuffer[ index + 3 ] = p0;
        obj->indexBuffer[ index + 4 ] = p1;
        obj->indexBuffer[ index + 5 ] = p3;
    }

    return obj;
}

