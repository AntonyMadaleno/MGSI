/**
 * @file Buffer.h
 * @brief Header for struct Buffer
 * @author Antony Madaleno
 * @version 1.0
 * @date 13-10-2023
 *
 * Header pour les struct Buffer
 *
 */

#pragma once

#ifndef BUFFER
#define BUFFER

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Object.h"

/**
 * @struct Buffer
 */
typedef struct Buffer
{
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    Object * object;
} Buffer;

/**
 * @brief initiate VAO, VBO and EBO
 * 
 * @param obj 
 * @return Buffer* 
 */
Buffer * Buffer_init(Object * obj);

/**
 * @brief 
 * 
 * @param obj 
 * @return void
 */
void Buffer_bind(Buffer * buf);

/**
 * @brief 
 * 
 * @param obj 
 * @return void
 */
void Buffer_draw(Buffer * buf);

#endif