#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>

typedef struct {
    GLuint program;
} Shader;

Shader shader_create(const char* vertex_path, const char* fragment_path);
void shader_use(Shader* shader);
void shader_delete(Shader* shader);

#endif