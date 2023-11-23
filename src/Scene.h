/**
 * @file Scene.h
 * @brief Header for struct Scene
 * @author Antony Madaleno
 * @version 1.0
 * @date 07-11-2023
 *
 * Header pour les struct Scenes
 *
 */

#pragma once

#include "Vec.h"
#include "Quaternion.h"
#include "Object.h"

typedef struct Scene
{
    Object * objects;
    unsigned int obj_count;
} Scene;

Scene * Scene_init();

void Scene_addObject(Scene * s, Object * obj);

Scene * Scene_fromFile(const char * filepath);