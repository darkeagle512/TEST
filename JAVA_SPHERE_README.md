# Java Animated Sphere

A Java Swing application that displays an animated rotating 3D sphere with lighting and shading.

## Features

- ✨ Rotating 3D sphere
- 🎨 Lighting and shading based on face normals
- 📐 Proper 3D perspective projection
- 🔄 Continuous smooth rotation on X, Y, and Z axes
- 💻 Pure Java with no external dependencies (uses built-in Swing/AWT)

## Requirements

- Java 8 or higher
- No external libraries needed (uses only standard Java libraries)

## Compilation & Execution

### On Windows, macOS, or Linux:

```bash
# Compile
javac AnimatedSphere.java

# Run
java AnimatedSphere
```

## How It Works

### 3D Sphere Generation
- Generates a sphere mesh using latitude/longitude method
- 30 sectors × 30 stacks (configurable for higher quality)
- Creates triangular faces for rendering

### 3D Transformations
- **Rotation**: Applies rotation matrices on X, Y, and Z axes
- **Projection**: Converts 3D coordinates to 2D screen coordinates using perspective projection
- **Animation**: Continuously updates rotation angles for smooth animation

### Rendering
- **Painter's Algorithm**: Sorts faces by depth (Z-coordinate) and draws back-to-front
- **Lighting**: Calculates face brightness based on surface normal and light direction
- **Shading**: Colors faces with cyan tones based on calculated brightness

## Code Structure

- `AnimatedSphere` - Main window class
- `SpherePanel` - Graphics rendering panel
- `Point3D` - 3D coordinate representation
- `Point2D` - 2D coordinate representation
- `Face` - Triangle face definition
- `FaceWithDepth` - Face with calculated depth for sorting

## Customization

Edit the constants in `SpherePanel`:

```java
private static final int SPHERE_RADIUS = 100;      // Size of sphere
private static final int SECTORS = 30;              // Horizontal resolution
private static final int STACKS = 30;               // Vertical resolution
```

Adjust rotation speeds in the animation thread:
```java
rotationX += 0.005f;  // X-axis rotation speed
rotationY += 0.007f;  // Y-axis rotation speed
rotationZ += 0.003f;  // Z-axis rotation speed
```

## Performance

- Runs at approximately 60 FPS
- Pure Java rendering (no GPU acceleration)
- Suitable for learning graphics programming concepts

## License

MIT License - Free to use and modify
