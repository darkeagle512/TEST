#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>

#include "sphere.h"

Sphere sphere_create(float radius, unsigned int sectors, unsigned int stacks)
{
    Sphere sphere;
    
    // Generate sphere vertices and indices
    float* vertices = (float*)malloc((sectors + 1) * (stacks + 1) * 3 * sizeof(float));
    unsigned int* indices = (unsigned int*)malloc(sectors * stacks * 6 * sizeof(unsigned int));
    
    unsigned int vertex_count = 0;
    unsigned int index_count = 0;
    
    float sector_step = 2 * 3.14159f / sectors;
    float stack_step = 3.14159f / stacks;
    
    // Generate vertices
    for (unsigned int i = 0; i <= stacks; ++i) {
        float stack_angle = 3.14159f / 2 - i * stack_step;
        float xy = radius * cosf(stack_angle);
        float z = radius * sinf(stack_angle);
        
        for (unsigned int j = 0; j <= sectors; ++j) {
            float sector_angle = j * sector_step;
            
            float x = xy * cosf(sector_angle);
            float y = xy * sinf(sector_angle);
            
            vertices[vertex_count++] = x;
            vertices[vertex_count++] = y;
            vertices[vertex_count++] = z;
        }
    }
    
    // Generate indices
    for (unsigned int i = 0; i < stacks; ++i) {
        unsigned int k1 = i * (sectors + 1);
        unsigned int k2 = k1 + sectors + 1;
        
        for (unsigned int j = 0; j < sectors; ++j, ++k1, ++k2) {
            if (i != 0) {
                indices[index_count++] = k1;
                indices[index_count++] = k2;
                indices[index_count++] = k1 + 1;
            }
            
            if (i != (stacks - 1)) {
                indices[index_count++] = k1 + 1;
                indices[index_count++] = k2;
                indices[index_count++] = k2 + 1;
            }
        }
    }
    
    sphere.index_count = index_count;
    
    // Create VAO and VBO
    glGenVertexArrays(1, &sphere.vao);
    glGenBuffers(1, &sphere.vbo);
    glGenBuffers(1, &sphere.ebo);
    
    glBindVertexArray(sphere.vao);
    
    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, sphere.vbo);
    glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(float), vertices, GL_STATIC_DRAW);
    
    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphere.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), indices, GL_STATIC_DRAW);
    
    // Vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    
    // Clean up
    free(vertices);
    free(indices);
    
    return sphere;
}

void sphere_render(Sphere* sphere)
{
    glBindVertexArray(sphere->vao);
    glDrawElements(GL_TRIANGLES, sphere->index_count, GL_UNSIGNED_INT, 0);
}

void sphere_delete(Sphere* sphere)
{
    glDeleteVertexArrays(1, &sphere->vao);
    glDeleteBuffers(1, &sphere->vbo);
    glDeleteBuffers(1, &sphere->ebo);
}