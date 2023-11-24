#include "Surface.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <string.h>

Surface3D * Surface3D_init( 
    Vec3 ** control_points,
    const unsigned char u_control_count,
    const unsigned char v_control_count,
    const unsigned short u_count, 
    const unsigned short v_count, 
    const enum methode mode_u, 
    const enum methode mode_v
)
{

    //Allocation du pointeur mémoire pour la surface
    Surface3D * surface = (Surface3D *) calloc(1, sizeof(Surface3D) );
    if (!surface) {
        fprintf(stderr, "Error: Memory allocation failed for Surface3D.\n");
        return NULL;
    }

    //on définie le nombre de point qui définiront la surface en fonction des directions u et v
    surface->N = u_count;
    surface->M = v_count;

    //Allocation du buffer de stockage des information de coordonnées
    surface->data = (float *) calloc((surface->N * surface->M * 3), sizeof(float) );
    if (!surface->data) {
        fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
        exit(EXIT_FAILURE);
    }

    //Allocation des buffer pour stocker les tangeantes et les normals
    surface->Tu = (Vec3 **) calloc(surface->N, sizeof(Vec3*));
    if (!surface->Tu) {
        fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
        exit(EXIT_FAILURE);
    }

    surface->Tv = (Vec3 **) calloc(surface->N, sizeof(Vec3*));
    if (!surface->Tv) {
        fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
        exit(EXIT_FAILURE);
    }

    surface->normals = (Vec3 **) calloc(surface->N, sizeof(Vec3*) );
    if (!surface->normals) {
        fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
        exit(EXIT_FAILURE);
    }

    //Allocation du buffer pour les points évaluer qui serviront de point de controle dans la prochaine étape
    Vec3 ** controls = ( Vec3 ** ) calloc(v_control_count, sizeof( Vec3 * ) );
    if (!controls) {
        fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
        exit(EXIT_FAILURE);
    }

    // Pour chaque point de controle dans la direction v
    for (unsigned char v = 0; v < v_control_count; v++)
    {
        // on récupère le pointeur vers les point de controles actuels
        Vec3 * current = control_points[v];

        // évaluation de la courbe
        Curve3D * c = Curve3D_init(current, u_control_count, surface->N, mode_u);

        //Allocation de la mémoire pour le stockage des points
        controls[v] = (Vec3 * ) calloc(surface->N, sizeof(Vec3) );
        if (!controls[v]) {
            fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
            exit(EXIT_FAILURE);
        }

        //Conversion des donnée du buffer de la courbes en point Vec3
        for (unsigned short i = 0; i < surface->N; i++)
        {
            controls[v][i].x = c->data[i * 3 + 0];
            controls[v][i].y = c->data[i * 3 + 1];
            controls[v][i].z = c->data[i * 3 + 2];
        }
    }

    //for each calculated spline in u direction based on v we generate the points of our surface
    for (unsigned char u = 0; u < surface->N; u++)
    {

        Vec3 * current_controls = (Vec3 *) calloc(v_control_count, sizeof(Vec3));
        if (!current_controls) {
            fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
            exit(EXIT_FAILURE);
        }


        for (unsigned char v = 0; v < v_control_count; v++)
            current_controls[v] = controls[v][u];

        Curve3D * c = Curve3D_init(current_controls, v_control_count, surface->M, mode_v);

        Curve3D_calculateTNB(c);

        surface->Tv[u] = (Vec3 *) calloc(surface->M, sizeof(Vec3) );
        if (!surface->Tv[u]) {
            fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
            exit(EXIT_FAILURE);
        }

        surface->Tu[u] = (Vec3 *) calloc( surface->M, sizeof(Vec3));
        if (!surface->Tu[u]) {
            fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
            exit(EXIT_FAILURE);
        }

        surface->normals[u] = (Vec3 *) calloc(surface->M, sizeof(Vec3) );
        if (!surface->normals[u]) {
            fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
            exit(EXIT_FAILURE);
        }

        memcpy(surface->data + u * surface->M * 3, c->data, sizeof(float) * c->npoints * 3 );
        memcpy(surface->Tv[u],      c->T, sizeof(Vec3) * surface->M );
        memcpy(surface->Tu[u],      c->B, sizeof(Vec3) * surface->M );
        memcpy(surface->normals[u], c->N, sizeof(Vec3) * surface->M );
    }

    return surface;

}

typedef struct Thread_args
{
    Surface3D * s;
    Object * obj;
    Vec3 * color;
    Vec3 * specular_color;
    float * shininess;
    float * reflection;
    unsigned char * layout;
    unsigned char * lsize;
    unsigned short i;
} Thread_args;

static void * thread_objectify(void * t_args)
{
    Thread_args * td = (Thread_args *) t_args;

    for (unsigned short j = 0; j < td->s->M; j++)
    {
        //POSITION
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 0] = td->s->data[3 * (td->i * td->s->M + j) + 0];
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 1] = td->s->data[3 * (td->i * td->s->M + j) + 1];
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 2] = td->s->data[3 * (td->i * td->s->M + j) + 2];

        //NORMAL
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 3] = td->s->normals[td->i][j].x;
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 4] = td->s->normals[td->i][j].y;
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 5] = td->s->normals[td->i][j].z;

        //COLORS PARAMETERS
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 6] = td->color->x;
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 7] = td->color->y;
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 8] = td->color->z;

        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 9]  = td->specular_color->x;
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 10] = td->specular_color->y;
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 11] = td->specular_color->z;

        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 12] = * ( td->shininess );
        td->obj->vertexBuffer[(* td->lsize) * (td->i * td->s->M + j) + 13] = * ( td->reflection );

        if (td->i < td->s->N - 2 && j < td->s->M - 2)
        {
            td->obj->indexBuffer[(td->i * td->s->M + j) * 6 + 0] = td->i * td->s->M + j;
            td->obj->indexBuffer[(td->i * td->s->M + j) * 6 + 1] = (td->i+1) * td->s->M + j;
            td->obj->indexBuffer[(td->i * td->s->M + j) * 6 + 2] = (td->i+1) * td->s->M + ( (j+1) % td->s->M );

            td->obj->indexBuffer[(td->i * td->s->M + j) * 6 + 3] = td->i * td->s->M + j;
            td->obj->indexBuffer[(td->i * td->s->M + j) * 6 + 4] = (td->i+1) * td->s->M + ( (j+1) % td->s->M );
            td->obj->indexBuffer[(td->i * td->s->M + j) * 6 + 5] = td->i * td->s->M + ( (j+1) % td->s->M );
        }

    }

    return NULL;
}

Object * Surface3D_obejctify(Surface3D * s, Vec3 * color, Vec3 * specular_color, float shininess, float reflection)
{

    unsigned char * layout = (unsigned char *) calloc(6, sizeof(unsigned char));
    if (!layout) {
        fprintf(stderr, "Error: Memory allocation failed for layout.\n");
        exit(EXIT_FAILURE);
    }

    layout[0] = 3;
    layout[1] = 3;
    layout[2] = 3;
    layout[3] = 3;
    layout[4] = 1;
    layout[5] = 1;

    unsigned char layout_size = 14;

    Object * obj = Object_init( 
        ( (unsigned int) s->N ) * ( (unsigned int) s->M ), 
        6, layout,
        ((unsigned int) s->N - 1) * ((unsigned int) s->M - 1) * 2
    );

    pthread_t * threads = (pthread_t *) calloc( s->N , sizeof(pthread_t) );
    if (!threads) {
        fprintf(stderr, "Error: Memory allocation failed for threads.\n");
        exit(EXIT_FAILURE);
    }

    Thread_args * t_args = (Thread_args *) calloc( s->N , sizeof(Thread_args));
    if (!t_args) {
        fprintf(stderr, "Error: Memory allocation failed for threads arguments.\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned short i = 0; i < s->N; i++)
    {
        t_args[i].color = color;
        t_args[i].layout = layout;
        t_args[i].lsize = &layout_size;
        t_args[i].obj = obj;
        t_args[i].reflection = &reflection;
        t_args[i].shininess = &shininess;
        t_args[i].specular_color = specular_color;
        t_args[i].s = s;
        t_args[i].i = i;

        pthread_create(&threads[i], NULL, thread_objectify, (void *) &t_args[i]);
    }

    for (unsigned short i = 0; i < s->N; i++)
        pthread_join(threads[i], NULL);

    free(t_args);
    free(threads);

    return obj;

}