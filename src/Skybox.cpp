#include "Skybox.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct thread_data {
    const char * path;
    unsigned char * data;
    GLsizei w;
    GLsizei h;
} thread_data;

static void * thread_readImage(void * th_data)
{
    thread_data * d = (thread_data *) th_data;
    Image * img = Image_import(d->path);
    d->w = img->width;
    d->h = img->height;

    d->data = Image_toArray(img);
    Image_free(img);

    return NULL;
}

Skybox * Skybox_init(const char ** fpath)
{
    Skybox * skybox = (Skybox *) calloc(1, sizeof(Skybox));
    skybox->faces_paths = fpath;
    skybox->data = (unsigned char **) calloc(6, sizeof(unsigned char *));

    GLsizei width; 
    GLsizei height;

    pthread_t * threads = (pthread_t *) calloc(6, sizeof(pthread_t));

    unsigned char i;
    thread_data * th_data = (thread_data *) calloc(6, sizeof(thread_data));

    for (i = 0; i < 6; i++)
    {
        th_data[i].path = skybox->faces_paths[i];
        pthread_create(&threads[i], NULL, thread_readImage, (void *) &th_data[i]);
    }

    for (unsigned char i = 0; i < 6; i++)
    {
        pthread_join(threads[i], NULL);
        skybox->data[i] = th_data[i].data;
    }

    width = th_data[0].w;
    height = th_data[1].h;

    free(th_data);
    free(threads);

    glGenTextures(1, &skybox->ID);
    glActiveTexture(GL_TEXTURE0 + skybox->ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->ID);

    for (unsigned char i = 0; i < 6; i++)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, skybox->data[i]);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return skybox;
}

void Skybox_free(Skybox * skybox)
{
    for (unsigned char i = 0; i < 6; i++)
        free(skybox->data[i]);

    free(skybox->data);
    free(skybox->faces_paths);
}