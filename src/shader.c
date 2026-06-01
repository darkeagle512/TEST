#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <GL/glew.h>

#include "shader.h"

static char* read_file(const char* path)
{
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open file: %s\n", path);
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long size = ftell(file);
    fseek(file, 0, SEEK_SET);

    char* buffer = (char*)malloc(size + 1);
    fread(buffer, 1, size, file);
    buffer[size] = '\0';

    fclose(file);
    return buffer;
}

Shader shader_create(const char* vertex_path, const char* fragment_path)
{
    Shader shader;
    shader.program = 0;

    // Read shader sources
    char* vertex_source = read_file(vertex_path);
    char* fragment_source = read_file(fragment_path);

    if (!vertex_source || !fragment_source) {
        fprintf(stderr, "Failed to read shader files\n");
        if (vertex_source) free(vertex_source);
        if (fragment_source) free(fragment_source);
        return shader;
    }

    // Compile vertex shader
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, (const char* const*)&vertex_source, NULL);
    glCompileShader(vertex_shader);

    // Check for vertex shader compile errors
    int success;
    char info_log[512];
    glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex_shader, 512, NULL, info_log);
        fprintf(stderr, "Vertex shader compilation failed:\n%s\n", info_log);
    }

    // Compile fragment shader
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, (const char* const*)&fragment_source, NULL);
    glCompileShader(fragment_shader);

    // Check for fragment shader compile errors
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, NULL, info_log);
        fprintf(stderr, "Fragment shader compilation failed:\n%s\n", info_log);
    }

    // Link shaders
    shader.program = glCreateProgram();
    glAttachShader(shader.program, vertex_shader);
    glAttachShader(shader.program, fragment_shader);
    glLinkProgram(shader.program);

    // Check for linking errors
    glGetProgramiv(shader.program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader.program, 512, NULL, info_log);
        fprintf(stderr, "Shader program linking failed:\n%s\n", info_log);
    }

    // Clean up
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    free(vertex_source);
    free(fragment_source);

    return shader;
}

void shader_use(Shader* shader)
{
    glUseProgram(shader->program);
}

void shader_delete(Shader* shader)
{
    glDeleteProgram(shader->program);
}