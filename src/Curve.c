#include "Curve.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// Function to recursively calculate a point on the Bezier curve
static Vec3 CalculateBezierPoint(Vec3* control_points, unsigned char num_points, const float t) {
    if (num_points == 1) {
        // Base case: Return the single control point
        return control_points[0];
    }

    // Create a temporary array to store the intermediate control points
    Vec3* temp_points = (Vec3*) calloc(num_points, sizeof(Vec3));
    if (!temp_points) {
        fprintf(stderr, "Error: Memory allocation failed for temporary points array.\n");
        exit(EXIT_FAILURE);
    }

    // Calculate the next set of control points
    for (unsigned short i = 0; i < num_points - 1; i++) {
        temp_points[i].x = (1.0f - t) * control_points[i].x + t * control_points[i + 1].x;
        temp_points[i].y = (1.0f - t) * control_points[i].y + t * control_points[i + 1].y;
        temp_points[i].z = (1.0f - t) * control_points[i].z + t * control_points[i + 1].z;
    }

    // Recursively calculate the point on the Bezier curve
    Vec3 result = CalculateBezierPoint(temp_points, num_points - 1, t);

    // Clean up the temporary array
    free(temp_points);

    return result;
}

// Function to generate the curve using the Casteljau recursive algorithm
static void Curve3D_bezier(Curve3D* c, Vec3* c_points, unsigned char k) {
    if (!c || !c_points) {
        fprintf(stderr, "Error: Invalid input arguments for Bezier curve generation.\n");
        return;
    }

    // Allocate memory for the generated curve data
    c->data = (float*) calloc(c->npoints * 3, sizeof(float));
    if (!c->data) {
        fprintf(stderr, "Error: Memory allocation failed for Bezier curve data.\n");
        return;
    }

    // Generate points along the Bezier curve
    for (unsigned short i = 0; i < c->npoints; i++) {
        float t = (float)i / (c->npoints - 1);
        Vec3 point = CalculateBezierPoint(c_points, k, t);
        c->data[i * 3] = point.x;
        c->data[i * 3 + 1] = point.y;
        c->data[i * 3 + 2] = point.z;
    }
}

// Function to generate the curve using the Catmull-Rom algorithm for a uniform B-spline
static void Curve3D_catmullRom(Curve3D* c, Vec3* c_points, unsigned char k) {
    if (!c || !c_points || k < 4) {
        fprintf(stderr, "Error: Invalid input arguments for Catmull-Rom curve generation.\n");
        return;
    }

    // Allocate memory for the generated curve data
    c->data = (float*) calloc(c->npoints * 3, sizeof(float));
    if (!c->data) {
        fprintf(stderr, "Error: Memory allocation failed for Catmull-Rom curve data.\n");
        return;
    }

    // Temporary variables for interpolated points
    Vec3 p0, p1, p2, p3;

    // Generate points along the Catmull-Rom B-spline
    for (int i = 0; i < c->npoints; i++) {
        float t = (float)i / (c->npoints - 1); // Adjusted range to [0, 1]
        unsigned char segment = (unsigned char)(t * (k - 3)); // Ensure valid range
        float segmentT = (t * (k - 3)) - (float)segment;

        // Calculate the four control points for the Catmull-Rom B-spline segment
        p0 = c_points[segment];
        p1 = c_points[segment + 1];
        p2 = c_points[segment + 2];
        p3 = c_points[segment + 3];

        // Interpolate using the Catmull-Rom formula
        float t2 = segmentT * segmentT;
        float t3 = t2 * segmentT;
        float h1 = -0.5f * t3 + t2 - 0.5f * segmentT;
        float h2 = 1.5f * t3 - 2.5f * t2 + 1.0f;
        float h3 = -1.5f * t3 + 2.0f * t2 + 0.5f * segmentT;
        float h4 = 0.5f * t3 - 0.5f * t2;

        Vec3* point = (Vec3*) calloc(1, sizeof(Vec3));
        if (!point) {
            fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
            exit(EXIT_FAILURE);
        }
        point->x = (p0.x * h1 + p1.x * h2 + p2.x * h3 + p3.x * h4);
        point->y = (p0.y * h1 + p1.y * h2 + p2.y * h3 + p3.y * h4);
        point->z = (p0.z * h1 + p1.z * h2 + p2.z * h3 + p3.z * h4);

        // Store the interpolated point in the curve data
        c->data[i * 3] = point->x;
        c->data[i * 3 + 1] = point->y;
        c->data[i * 3 + 2] = point->z;

        free(point);
    }
}

// Function to generate the curve using the NUBS algorithm
static void Curve3D_nubs(Curve3D * c, Vec3 * c_points, float * nodals, unsigned char n, unsigned char degree) {
    
    unsigned char k = degree + 1;

    // Allocate memory for the generated curve data
    c->data = (float*) calloc(c->npoints * 3, sizeof(float) );
    if (!c->data) {
        fprintf(stderr, "Error: Memory allocation failed for NUBS curve data.\n");
        return;
    }

    //calculate the number of nodal vectors
    unsigned short node_count = (unsigned short) n + k;
    unsigned short subspline_count = (unsigned short) n - k + 2;

    //domain of definition of t will go from 0 to definition
    float definition = 0.0f;

    unsigned short i;
    for (i = 0; i < node_count; i++)
        definition += nodals[i];

    for (i = 0; i < c->npoints; i++)
    {
        // Allocate memory to temporarly store the point to interpolate
        Vec3 * point = (Vec3 *) calloc(1, sizeof(Vec3));
        if (!point) {
            fprintf(stderr, "Error: Memory allocation failed for buffer array.\n");
            exit(EXIT_FAILURE);
        }

        point->x = 0.0f;
        point->y = 0.0f;
        point->z = 0.0f;



        // Store the interpolated point in the curve data
        c->data[i * 3] = point->x;
        c->data[i * 3 + 1] = point->y;
        c->data[i * 3 + 2] = point->z;

        //free the temporary point
        free(point);
    }

}


// Function to initialize a Curve3D
Curve3D* Curve3D_init(Vec3* control_points, unsigned char n_control_points , const unsigned short n_points, const enum methode mode) {
    Curve3D* curve = (Curve3D*) calloc(1, sizeof(Curve3D));
    if (!curve) {
        fprintf(stderr, "Error: Memory allocation failed for Curve3D.\n");
        return NULL;
    }

    curve->npoints = n_points;
    curve->data = (float*) calloc(n_points * 3, sizeof(float) ); // 3 components (x, y, z) per point
    if (!curve->data) {
        fprintf(stderr, "Error: Memory allocation failed for Curve3D data.\n");
        free(curve);
        return NULL;
    }

    // Generate the curve based on the selected method
    switch (mode) {
        case BEZIER:
            Curve3D_bezier(curve, control_points, n_control_points);
            break;
        case CATMULL_ROM:
            Curve3D_catmullRom(curve, control_points, n_control_points);
            break;
        case NUBS:
            //Curve3D_nubs(curve, control_points, n_control_points);
            break;
        default:
            fprintf(stderr, "Error: Invalid method selected for curve generation.\n");
            free(curve->data);
            free(curve);
            return NULL;
    }

    return curve;
}

typedef struct thread_args
{
    Curve3D * c;
    unsigned short i;
    unsigned char n;
    unsigned char m;
    Object * o;
    Vec3 * color; 
    Vec3 * specular_color; 
    float shininess; 
    float reflectivness;
    float radius;
} thread_args;

static void * thread_fn_T(void * args)
{
    thread_args * th_d = (thread_args *) args;
    Curve3D * c = th_d->c;
    unsigned char n = th_d->n;
    unsigned short i = th_d->i * n + 1;

    for (unsigned char x = 0; x < n; x++)
    {
        if (i + x < c->npoints - 1)
        {
            c->T[(i + x)].x = (c->data[3 * (i + x) + 0] - c->data[3 * ((i + x)-1) + 0] + c->data[3 * ((i + x)+1) + 0] - c->data[3 * (i + x) + 0]) / 2.0f;
            c->T[(i + x)].y = (c->data[3 * (i + x) + 1] - c->data[3 * ((i + x)-1) + 1] + c->data[3 * ((i + x)+1) + 1] - c->data[3 * (i + x) + 1]) / 2.0f;
            c->T[(i + x)].z = (c->data[3 * (i + x) + 2] - c->data[3 * ((i + x)-1) + 2] + c->data[3 * ((i + x)+1) + 2] - c->data[3 * (i + x) + 2]) / 2.0f;
            Vec3_normalize(&c->T[(i + x)]);
        }
    }

    return NULL;
}

static void calc_T(Curve3D * c)
{
    //first point
    c->T[0].x = c->data[3] - c->data[0];
    c->T[0].y = c->data[4] - c->data[1];
    c->T[0].z = c->data[5] - c->data[2];
    Vec3_normalize(&c->T[0]);

    unsigned char n_round = 1 + (c->npoints - 2) / PTHREAD_THREADS_MAX;

    pthread_t * threads = (pthread_t *) calloc( __min(PTHREAD_THREADS_MAX, c->npoints - 2) , sizeof(pthread_t) );
    if (!threads) {
        fprintf(stderr, "Error: Memory allocation failed for threads.\n");
        exit(EXIT_FAILURE);
    }

    thread_args * t_args = (thread_args *) calloc( __min(PTHREAD_THREADS_MAX, c->npoints - 2) , sizeof(thread_args));
    if (!t_args) {
        fprintf(stderr, "Error: Memory allocation failed for threads arguments.\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned short t = 0; t < __min(PTHREAD_THREADS_MAX, c->npoints - 2); t++)
    {
        t_args[t].c = c;
        t_args[t].n = n_round;
        t_args[t].i = t;
        pthread_create(&threads[t], NULL, thread_fn_T, (void *) &t_args[t]);
    }

    //last point
    c->T[c->npoints - 1].x = c->data[3 * (c->npoints - 1) + 0] - c->data[3 * (c->npoints - 2) + 0];
    c->T[c->npoints - 1].y = c->data[3 * (c->npoints - 1) + 1] - c->data[3 * (c->npoints - 2) + 1];
    c->T[c->npoints - 1].z = c->data[3 * (c->npoints - 1) + 2] - c->data[3 * (c->npoints - 2) + 2];
    Vec3_normalize(&c->T[c->npoints - 1]);

    for (unsigned short t = 0; t < __min(PTHREAD_THREADS_MAX, c->npoints - 2); t++)
        pthread_join(threads[t], NULL);

    free(t_args);
    free(threads);
}

static void calc_N(Curve3D * c)
{
    //first point
    c->N[0].x = c->T[1].x - c->T[0].x;
    c->N[0].y = c->T[1].y - c->T[0].y;
    c->N[0].z = c->T[1].z - c->T[0].z;
    Vec3_normalize(&c->N[0]);

    for (unsigned short i = 1; i < c->npoints - 1; i++)
    {
        c->N[i].x = (c->T[i+1].x - c->T[i].x + c->T[i].x - c->T[i-1].x) / 2.0f;
        c->N[i].y = (c->T[i+1].y - c->T[i].y + c->T[i].y - c->T[i-1].y) / 2.0f;
        c->N[i].z = (c->T[i+1].z - c->T[i].z + c->T[i].z - c->T[i-1].z) / 2.0f;

        if (Vec3_length2(&c->N[i]) == 0)
        {
            c->N[i].x = c->N[i-1].x;
            c->N[i].y = c->N[i-1].y;
            c->N[i].z = c->N[i-1].z;
        }
        else
        {
            Vec3_normalize(&c->N[i]);
        }
        
    }

    //last point
    c->N[c->npoints-1].x = c->T[c->npoints-1].x - c->T[c->npoints-2].x;
    c->N[c->npoints-1].y = c->T[c->npoints-1].y - c->T[c->npoints-2].y;
    c->N[c->npoints-1].z = c->T[c->npoints-1].z - c->T[c->npoints-2].z;
    Vec3_normalize(&c->N[c->npoints-1]);
}

void Curve3D_calculateTNB(Curve3D * c)
{
    c->T = (Vec3 *) calloc(c->npoints, sizeof(Vec3));
    if (!c->T) {
        fprintf(stderr, "Error: Memory allocation failed for Curve3D T.\n");
        exit(EXIT_FAILURE);
    }

    calc_T(c);

    c->N = (Vec3 *) calloc(c->npoints, sizeof(Vec3));
    if (!c->N) {
        fprintf(stderr, "Error: Memory allocation failed for Curve3D N.\n");
        exit(EXIT_FAILURE);
    }

    calc_N(c);

    c->B = (Vec3 *) calloc(c->npoints, sizeof(Vec3));
    if (!c->B) {
        fprintf(stderr, "Error: Memory allocation failed for Curve3D B.\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned short i = 0; i < c->npoints; i++)
        c->B[i] = * Vec3_cross(&c->T[i], &c->N[i]);

}

static void * thread_fn_Surface(void * args)
{
    unsigned char s = 14;

    thread_args * th_d = (thread_args *) args;
    Curve3D * c = th_d->c;
    unsigned char n = th_d->n;
    unsigned char meridians = th_d->m;
    unsigned short t = th_d->i * n;
    Object * surface = th_d->o;
    Vec3 * color = th_d->color; 
    Vec3 * specular_color = th_d->specular_color; 
    float shininess = th_d->shininess; 
    float reflection = th_d->reflectivness;
    float radius = th_d->radius;

    for (unsigned short i = t; i < t + n; i++)
    {
        if (i < c->npoints)
            for (unsigned char j = 0; j < meridians; j++)
            {
                float angle = 360.0f * ( (float) j ) / ( (float) meridians );
                Quaternion * rotation = Quaternion_fromAxisAngle(angle, &c->T[i]);
                Vec3 * v = Quaternion_RotateVector(&c->N[i], rotation);
                Vec3_normalize(v);

                //POSITION
                surface->vertexBuffer[s * (i * meridians + j) + 0] = c->data[i*3 + 0] + radius * v->x;
                surface->vertexBuffer[s * (i * meridians + j) + 1] = c->data[i*3 + 1] + radius * v->y;
                surface->vertexBuffer[s * (i * meridians + j) + 2] = c->data[i*3 + 2] + radius * v->z;

                //NORMAL
                surface->vertexBuffer[s * (i * meridians + j) + 3] = v->x;
                surface->vertexBuffer[s * (i * meridians + j) + 4] = v->y;
                surface->vertexBuffer[s * (i * meridians + j) + 5] = v->z;

                free(rotation);
                free(v);

                //COLORS
                surface->vertexBuffer[s * (i * meridians + j) + 6] = color->x;
                surface->vertexBuffer[s * (i * meridians + j) + 7] = color->y;
                surface->vertexBuffer[s * (i * meridians + j) + 8] = color->z;

                surface->vertexBuffer[s * (i * meridians + j) + 9]  = specular_color->x;
                surface->vertexBuffer[s * (i * meridians + j) + 10] = specular_color->y;
                surface->vertexBuffer[s * (i * meridians + j) + 11] = specular_color->z;

                surface->vertexBuffer[s * (i * meridians + j) + 12] = shininess;
                surface->vertexBuffer[s * (i * meridians + j) + 13] = reflection;

                //INDEX
                if (i < c->npoints - 1)
                {
                    surface->indexBuffer[(i * meridians + j) * 6 + 0] = i * meridians + j;
                    surface->indexBuffer[(i * meridians + j) * 6 + 1] = (i+1) * meridians + j;
                    surface->indexBuffer[(i * meridians + j) * 6 + 2] = (i+1) * meridians + ( (j+1) % meridians );

                    surface->indexBuffer[(i * meridians + j) * 6 + 3] = i * meridians + j;
                    surface->indexBuffer[(i * meridians + j) * 6 + 4] = (i+1) * meridians + ( (j+1) % meridians );
                    surface->indexBuffer[(i * meridians + j) * 6 + 5] = i * meridians + ( (j+1) % meridians );
                }
                    
            }
    }

    return NULL;
}

Object * Curve3D_generateSurface(Curve3D * c, unsigned char meridians, Vec3 * color, Vec3 * specular_color, float shininess, float reflection,
 float radius)
{
    if (c->T == NULL)
    {
        Curve3D_calculateTNB(c);
    }

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

    Object * surface = Object_init( 
        ( (unsigned int) c->npoints ) * ( (unsigned int) meridians ), 
        6, layout,
        ( ( (unsigned int) c->npoints) - 1) * ( (unsigned int) meridians ) * 2
    );

    unsigned char n_round = 1 + (c->npoints) / PTHREAD_THREADS_MAX;

    pthread_t * threads = (pthread_t *) calloc(__min(PTHREAD_THREADS_MAX, c->npoints), sizeof(pthread_t) );
    if (!threads) {
        fprintf(stderr, "Error: Memory allocation failed for threads.\n");
        exit(EXIT_FAILURE);
    }

    thread_args * t_args = (thread_args *) calloc(__min(PTHREAD_THREADS_MAX, c->npoints), sizeof(thread_args) );
    if (!t_args) {
        fprintf(stderr, "Error: Memory allocation failed for threads arguments.\n");
        exit(EXIT_FAILURE);
    }

    for (unsigned short t = 0; t < __min(PTHREAD_THREADS_MAX, c->npoints); t++)
    {
        t_args[t].c = c;
        t_args[t].n = n_round;
        t_args[t].i = t;
        t_args[t].m = meridians;
        t_args[t].o = surface;
        t_args[t].color = color;
        t_args[t].radius = radius;
        t_args[t].reflectivness = reflection;
        t_args[t].shininess = shininess;
        t_args[t].specular_color = specular_color;

        pthread_create(&threads[t], NULL, thread_fn_Surface, (void *) &t_args[t]);
    }

    for (unsigned short t = 0; t < __min(PTHREAD_THREADS_MAX, c->npoints - 2); t++)
        pthread_join(threads[t], NULL);

    free(threads);
    free(t_args);
    return surface;

}