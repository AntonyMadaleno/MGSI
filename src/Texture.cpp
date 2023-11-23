#include "Texture.h"
#include <stdio.h>
#include <stdlib.h>

Texture * Texture_init(const char * fpath)
{
    Texture * texture = (Texture *) calloc(1, sizeof(Texture));
    if (!texture) {
        fprintf(stderr, "Error: Memory allocation failed for texture.\n");
        exit(EXIT_FAILURE);
    }
    texture->face_path = fpath;

    Image * tx_img = Image_import(fpath);

    GLsizei width = tx_img->width; 
    GLsizei height = tx_img->height;

    texture->data = Image_toArray(tx_img);
    Image_free(tx_img);

    glGenTextures(1, &texture->ID);
    glActiveTexture(GL_TEXTURE0 + texture->ID);
    glBindTexture(GL_TEXTURE_2D, texture->ID);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture->data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_2D);
    return texture;
}

void Texture_free(Texture * texture)
{
    free(texture->data);
    free(texture);
}