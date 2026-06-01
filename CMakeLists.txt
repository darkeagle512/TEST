cmake_minimum_required(VERSION 3.10)
project(OpenGL_Sphere)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find required packages
find_package(OpenGL REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)

# Include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.c
    src/shader.c
    src/sphere.c
)

# Create executable
add_executable(opengl_sphere ${SOURCES})

# Link libraries
target_link_libraries(opengl_sphere
    ${OPENGL_LIBRARIES}
    GLFW::glfw
    GLEW::GLEW
)

# Copy shader files to build directory
add_custom_command(TARGET opengl_sphere POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/shaders
    $<TARGET_FILE_DIR:opengl_sphere>/shaders
)