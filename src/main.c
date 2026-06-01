#include <stdio.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "shader.h"
#include "sphere.h"

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);

int main(void)
{
    // Initialize GLFW
    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create window
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "OpenGL Sphere", NULL, NULL);
    if (!window) {
        fprintf(stderr, "Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSwapInterval(1); // Enable vsync

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW: %s\n", glewGetErrorString(err));
        glfwTerminate();
        return -1;
    }

    printf("OpenGL Version: %s\n", glGetString(GL_VERSION));

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    // Create shader program
    Shader shader = shader_create("shaders/vertex.glsl", "shaders/fragment.glsl");

    // Create sphere
    Sphere sphere = sphere_create(1.0f, 50, 50);

    // Main render loop
    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        // Clear buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use shader
        shader_use(&shader);

        // Set uniforms
        float time = (float)glfwGetTime();
        
        // Model matrix (rotate the sphere)
        float model[16] = {
            cosf(time), 0, sinf(time), 0,
            0, 1, 0, 0,
            -sinf(time), 0, cosf(time), 0,
            0, 0, 0, 1
        };

        // View matrix (camera looking at sphere)
        float view[16] = {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            0, 0, -3, 1
        };

        // Projection matrix (perspective)
        float fov = 45.0f * 3.14159f / 180.0f;
        float aspect = (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT;
        float f = 1.0f / tanf(fov / 2.0f);
        float projection[16] = {
            f / aspect, 0, 0, 0,
            0, f, 0, 0,
            0, 0, -1.01f, -1,
            0, 0, -2.01f, 0
        };

        GLint model_loc = glGetUniformLocation(shader.program, "model");
        GLint view_loc = glGetUniformLocation(shader.program, "view");
        GLint proj_loc = glGetUniformLocation(shader.program, "projection");

        glUniformMatrix4fv(model_loc, 1, GL_FALSE, model);
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, view);
        glUniformMatrix4fv(proj_loc, 1, GL_FALSE, projection);

        // Render sphere
        sphere_render(&sphere);

        // Swap buffers and poll events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    sphere_delete(&sphere);
    shader_delete(&shader);
    glfwTerminate();

    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}