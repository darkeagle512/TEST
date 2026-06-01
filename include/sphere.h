#ifndef SPHERE_H
#define SPHERE_H

#include <GL/glew.h>

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    unsigned int index_count;
} Sphere;

Sphere sphere_create(float radius, unsigned int sectors, unsigned int stacks);
void sphere_render(Sphere* sphere);
void sphere_delete(Sphere* sphere);

#endif