#include "Scene.h"
#include <stdio.h>
#include <stdlib.h>

Scene * Scene_init()
{
    Scene * s = (Scene *) malloc(sizeof(Scene));
    s->obj_count = 0;

    return s;
}

void Scene_addObject(Scene * s, Object * obj)
{
    if (s->objects == NULL)
        s->objects = malloc(sizeof(Object));
    else
        realloc(s->objects, sizeof(Object) * s->obj_count + 1);

    s->objects[s->obj_count] = *obj;
    s->obj_count++;
}

Scene * Scene_fromFile(const char * filepath)
{

}