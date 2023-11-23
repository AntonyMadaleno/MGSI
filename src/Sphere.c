#include "Sphere.h"
#include "pthread.h"
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

Sphere * Sphere_init(Vec3 * position, float rx, float ry, float rz)
{
    Sphere * s = (Sphere *) calloc(1, sizeof(Sphere) );
    s->position = position;
    s->rx = rx;
    s->ry = ry;
    s->rz = rz;

    return s;
}

typedef struct Thread_args
{
    unsigned char m;
    unsigned char p;
    unsigned char n;
    Object * o;
    Sphere * s;
    float theta;
    Vec3 * color;
    Vec3 * specular;
    float shine;
    float refl;
} Thread_args;

void * thread_drawParallel(void * args)
{
    Thread_args * t_args = (Thread_args *) args;
    unsigned char m = t_args->m;
    unsigned char n = t_args->n;
    unsigned char p = t_args->p;
    Object * o = t_args->o;
    Sphere * s = t_args->s;
    float theta = t_args->theta;
    Vec3 * color = t_args->color;
    Vec3 * specular = t_args->specular;
    float shine = t_args->shine;
    float refl = t_args->refl;

    unsigned short offset = p * m + 1;
    unsigned int size = 14;

    for (unsigned char i = 0; i < m; i++)
    {
        //vertex
        float alpha = ((float) i) / ((float) m) * 2 * M_PI;
        o->vertexBuffer[(offset + i) * size + 0] = s->position->x + s->rx * cosf( theta ) * cosf( alpha );
        o->vertexBuffer[(offset + i) * size + 2] = s->position->z + s->rz * cosf( theta ) * sinf( alpha );
        o->vertexBuffer[(offset + i) * size + 1] = s->position->y + s->ry * sinf( theta );

        o->vertexBuffer[(offset + i) * size + 3] = o->vertexBuffer[(offset + i) * size + 0] - s->position->x;
        o->vertexBuffer[(offset + i) * size + 4] = o->vertexBuffer[(offset + i) * size + 1] - s->position->y;
        o->vertexBuffer[(offset + i) * size + 5] = o->vertexBuffer[(offset + i) * size + 2] - s->position->z;

        o->vertexBuffer[(offset + i) * size + 6] = color->x;
        o->vertexBuffer[(offset + i) * size + 7] = color->y;
        o->vertexBuffer[(offset + i) * size + 8] = color->z;

        o->vertexBuffer[(offset + i) * size + 9] = specular->x;
        o->vertexBuffer[(offset + i) * size + 10] = specular->y;
        o->vertexBuffer[(offset + i) * size + 11] = specular->z;

        o->vertexBuffer[(offset + i) * size + 12] = shine;
        o->vertexBuffer[(offset + i) * size + 13] = refl;

        //index
        unsigned in = (i+1)% m;
        unsigned jn = (p+1)% n;

        unsigned p0 = p  * m + i  + 1;
        unsigned p1 = p  * m + in + 1;
        unsigned p2 = jn * m + in + 1;
        unsigned p3 = jn * m + i  + 1;

        unsigned int index = ((unsigned int) m * 3) + ((unsigned int) m * p * 6) + ((unsigned int) i * 6);

        o->indexBuffer[ index + 0 ] = p0;
        o->indexBuffer[ index + 1 ] = p3;
        o->indexBuffer[ index + 2 ] = p2;

        o->indexBuffer[ index + 3 ] = p0;
        o->indexBuffer[ index + 4 ] = p2;
        o->indexBuffer[ index + 5 ] = p1;
    }

    return NULL;
}

Object * Sphere_generateSurface(
    Sphere * sphere,
    unsigned char meridian,
    unsigned char parallel,
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

    //CREATE THE OBJECT
    Object * obj = Object_init( meridian * parallel + 2, 6, layout, 2 * meridian * (parallel + 1) );

    //CREATE POLES
    Vec3 * north = (Vec3 *) calloc(1, sizeof(Vec3) );     
    Vec3_set(north, sphere->position->x, sphere->position->y + sphere->ry, sphere->position->z);

    unsigned int size = 14;

    obj->vertexBuffer[(obj->n_points - 1) * size - 1]  = north->x;
    obj->vertexBuffer[(obj->n_points - 1) * size - 0]  = north->z;
    obj->vertexBuffer[(obj->n_points - 1) * size + 1]  = north->y;

    obj->vertexBuffer[(obj->n_points - 1) * size + 2]  = north->x - sphere->position->x;
    obj->vertexBuffer[(obj->n_points - 1) * size + 3]  = north->z - sphere->position->z;
    obj->vertexBuffer[(obj->n_points - 1) * size + 4]  = north->y - sphere->position->y;

    obj->vertexBuffer[(obj->n_points - 1) * size + 5]  = color->x;
    obj->vertexBuffer[(obj->n_points - 1) * size + 6]  = color->y;
    obj->vertexBuffer[(obj->n_points - 1) * size + 7]  = color->z;

    obj->vertexBuffer[(obj->n_points - 1) * size + 8]  = specular->x;
    obj->vertexBuffer[(obj->n_points - 1) * size + 9]  = specular->y;
    obj->vertexBuffer[(obj->n_points - 1) * size + 10] = specular->z;

    obj->vertexBuffer[(obj->n_points - 1) * size + 11] = shininess;
    obj->vertexBuffer[(obj->n_points - 1) * size + 12] = reflection;

    Vec3 * south = (Vec3 *) calloc(1, sizeof(Vec3) );
    Vec3_set(south, sphere->position->x, sphere->position->y - sphere->ry, sphere->position->z);

    obj->vertexBuffer[0]  = south->x;
    obj->vertexBuffer[1]  = south->z;
    obj->vertexBuffer[2]  = south->y;

    obj->vertexBuffer[3]  = south->x - sphere->position->x;
    obj->vertexBuffer[4]  = south->z - sphere->position->z;
    obj->vertexBuffer[5]  = south->y - sphere->position->y;

    obj->vertexBuffer[6]  = color->x;
    obj->vertexBuffer[7]  = color->y;
    obj->vertexBuffer[8]  = color->z;

    obj->vertexBuffer[9]  = specular->x;
    obj->vertexBuffer[10] = specular->y;
    obj->vertexBuffer[11] = specular->z;

    obj->vertexBuffer[12] = shininess;
    obj->vertexBuffer[13] = reflection;

    free(south);
    free(north);

    unsigned char i;
    for (i = 0; i < meridian; i++)
    {
        obj->indexBuffer[i*3 + 0] = 0;
        obj->indexBuffer[i*3 + 1] = 1 + i;
        obj->indexBuffer[i*3 + 2] = 1 + (i+1) % meridian;

        obj->indexBuffer[obj->n_faces * 3 - i*3 - 3] = obj->n_points - 1;
        obj->indexBuffer[obj->n_faces * 3 - i*3 - 2] = obj->n_points - 2 - i;
        obj->indexBuffer[obj->n_faces * 3 - i*3 - 1] = obj->n_points - 2 - ((i+1) % meridian);
    }

    //CREATE EACH PARALLELS
    pthread_t * threads = (pthread_t *) calloc(parallel, sizeof(pthread_t) );
    Thread_args * t_args = (Thread_args *) calloc(parallel, sizeof(Thread_args) );

    for (i = 0; i < parallel; i++)
    {
        t_args[i].m = meridian;
        t_args[i].p = i;
        t_args[i].n = parallel;
        t_args[i].o = obj;
        t_args[i].s = sphere;
        t_args[i].refl = reflection;
        t_args[i].shine = shininess;
        t_args[i].color = color;
        t_args[i].specular = specular;
        t_args[i].theta = ( (float) i ) / ( (float) parallel + 2) * M_PI - M_PI/2;

        pthread_create(&threads[i], NULL, thread_drawParallel, (void *) &t_args[i]);
    }

    for (i = 0; i < parallel; i++)
        pthread_join(threads[i], NULL);

    free(threads);
    free(t_args);
    return obj;
}

