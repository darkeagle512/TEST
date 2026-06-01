# OpenGL Sphere Renderer

A simple OpenGL application written in C that displays a rotating 3D sphere.

## Features

- OpenGL 3.3+ (Core Profile)
- GLFW for window management
- GLEW for OpenGL extensions
- Sphere mesh generation with configurable resolution
- Rotating sphere visualization
- Simple shader system

## Requirements

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install libglfw3-dev libglew-dev mesa-common-dev
```

### macOS
```bash
brew install glfw3 glew
```

### Windows
Install GLFW and GLEW using vcpkg or download precompiled binaries.

## Building

```bash
mkdir build
cd build
cmake ..
make
```

## Running

```bash
./opengl_sphere
```

Press `ESC` to close the application.

## Project Structure

```
.
├── CMakeLists.txt          # Build configuration
├── src/
│   ├── main.c             # Main application and render loop
│   ├── shader.c           # Shader compilation and management
│   └── sphere.c           # Sphere mesh generation and rendering
├── include/
│   ├── shader.h
│   └── sphere.h
├── shaders/
│   ├── vertex.glsl        # Vertex shader
│   └── fragment.glsl      # Fragment shader
└── README.md
```

## Code Overview

### main.c
- GLFW window initialization
- OpenGL context setup
- Main render loop
- Matrix transformations (model, view, projection)
- Sphere rotation using time

### sphere.c
- Sphere mesh generation using latitude/longitude method
- VAO, VBO, and EBO management
- Configurable sectors and stacks for detail level

### shader.c
- Shader file loading
- Vertex and fragment shader compilation
- Program linking
- Error checking and reporting

## Controls

- **ESC** - Exit application

## Customization

You can modify several parameters:

- **Sphere resolution**: Change `sectors` and `stacks` in `main.c` (sphere_create call)
- **Rotation speed**: Modify the `time` variable multiplication in the model matrix
- **Sphere color**: Edit the color in `shaders/fragment.glsl`
- **Camera distance**: Change the z-coordinate in the view matrix
- **Field of view**: Adjust the `fov` variable in `main.c`

## License

MIT License - Feel free to use and modify this code.