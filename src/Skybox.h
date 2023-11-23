/**
 * @file Skybox.h
 * @brief Header for struct Skybox
 * @author Antony Madaleno
 * @version 1.0
 * @date 14-10-2023
 *
 * Header pour les struct Skybox
 *
 */

#pragma once

#ifndef SKYBOX
#define SKYBOX

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Shader.hpp"
#include "Image.h"
#include <string>

/**
 * @struct Buffer
 */
typedef struct Skybox
{
    GLuint ID;
    const char ** faces_paths;
    unsigned char ** data;
} Skybox;

/**
 * @brief initiate Skybox
 * 
 */
Skybox * Skybox_init(const char ** fpath);

void Skybox_free(Skybox * skybox);

#endif