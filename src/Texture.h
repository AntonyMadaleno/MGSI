/**
 * @file Texture.h
 * @brief Header for struct Texture
 * @author Antony Madaleno
 * @version 1.0
 * @date 19-10-2023
 *
 * Header pour les struct Texture
 *
 */

#pragma once

#ifndef TEXTURE
#define TEXTURE

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Image.h"
#include <string>

/**
 * @struct Buffer
 */
typedef struct Texture
{
    GLuint ID;
    const char * face_path;
    unsigned char * data;
} Texture;

/**
 * @brief initiate Texture
 * 
 */
Texture * Texture_init(const char * fpath);

void Texture_free(Texture * tx);

#endif