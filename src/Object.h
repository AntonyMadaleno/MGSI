/**
 * @file Object.h
 * @brief Header for struct Object
 * @author Antony Madaleno
 * @version 1.0
 * @date 21-09-2023
 *
 * Header pour les struct Object
 *
 */

#pragma once

/**
 * @struct Object
 */
typedef struct Object
{
    unsigned int n_points;
    unsigned char n_attributes;
    unsigned char * layout;
    unsigned int n_faces;
    float * vertexBuffer;
    unsigned int * indexBuffer;
} Object;

/**
 * @brief initialize my object structure and allocate the memory
 * 
 * @param n_point 
 * @param n_faces
 * @param n_attributes 
 * @param a_layout
 * @return Object* 
 */
Object * Object_init(const unsigned int n_points, const unsigned char n_attributes, unsigned char * layout, const unsigned int n_faces);