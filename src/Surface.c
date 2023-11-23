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

    for (unsigned short i = 0; i < s->N; i++)
    {
        for (unsigned short j = 0; j < s->M; j++)
        {
            //POSITION
            obj->vertexBuffer[layout_size * (i * s->M + j) + 0] = s->data[3 * (i * s->M + j) + 0];
            obj->vertexBuffer[layout_size * (i * s->M + j) + 1] = s->data[3 * (i * s->M + j) + 1];
            obj->vertexBuffer[layout_size * (i * s->M + j) + 2] = s->data[3 * (i * s->M + j) + 2];

            //NORMAL
            obj->vertexBuffer[layout_size * (i * s->M + j) + 3] = s->normals[i][j].x;
            obj->vertexBuffer[layout_size * (i * s->M + j) + 4] = s->normals[i][j].y;
            obj->vertexBuffer[layout_size * (i * s->M + j) + 5] = s->normals[i][j].z;

            //COLORS PARAMETERS
            obj->vertexBuffer[layout_size * (i * s->M + j) + 6] = color->x;
            obj->vertexBuffer[layout_size * (i * s->M + j) + 7] = color->y;
            obj->vertexBuffer[layout_size * (i * s->M + j) + 8] = color->z;

            obj->vertexBuffer[layout_size * (i * s->M + j) + 9]  = specular_color->x;
            obj->vertexBuffer[layout_size * (i * s->M + j) + 10] = specular_color->y;
            obj->vertexBuffer[layout_size * (i * s->M + j) + 11] = specular_color->z;

            obj->vertexBuffer[layout_size * (i * s->M + j) + 12] = shininess;
            obj->vertexBuffer[layout_size * (i * s->M + j) + 13] = reflection;

            if (i < s->N - 2 && j < s->M - 2)
            {
                obj->indexBuffer[(i * s->M + j) * 6 + 0] = i * s->M + j;
                obj->indexBuffer[(i * s->M + j) * 6 + 1] = (i+1) * s->M + j;
                obj->indexBuffer[(i * s->M + j) * 6 + 2] = (i+1) * s->M + ( (j+1) % s->M );

                obj->indexBuffer[(i * s->M + j) * 6 + 3] = i * s->M + j;
                obj->indexBuffer[(i * s->M + j) * 6 + 4] = (i+1) * s->M + ( (j+1) % s->M );
                obj->indexBuffer[(i * s->M + j) * 6 + 5] = i * s->M + ( (j+1) % s->M );
            }

        }
    }

    return obj;

}