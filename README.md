# Black Hole Raytracer

A C++ raytracing engine that simulates gravitational lensing effects around black holes using general relativity principles. This project demonstrates advanced computer graphics techniques, physics simulation, and mathematical programming.

## Overview

This raytracer implements a simplified Schwarzschild metric to simulate how light bends around massive objects, creating realistic visualizations of black holes with accretion disks. The system uses custom 3D mathematics, adaptive ray marching, and temperature-based rendering to produce scientifically-inspired imagery.

## Technical Features

### Physics Implementation
- **Schwarzschild Metric**: Basic general relativity calculations for non-rotating black holes
- **Gravitational Lensing**: Ray bending simulation around massive objects
- **Event Horizon Rendering**: Accurate boundary calculation and rendering
- **Accretion Disk Physics**: Temperature-based coloring with orbital velocity effects

### Graphics Engine
- **Custom 3D Math Library**: Vector operations, matrix transformations, and geometric calculations
- **Ray Marching Algorithm**: Adaptive step sizing for performance optimization
- **Camera System**: Configurable field of view, position, and orientation
- **Post-Processing**: Gamma correction, contrast enhancement, and color clamping

### Performance Optimizations
- **Adaptive Ray Marching**: Variable step sizes based on gravitational field strength
- **Early Termination**: Skip calculations beyond event horizon
- **Memory Management**: Efficient data structures for large-scale rendering
- **Multi-threading Ready**: Architecture designed for parallel processing

## Project Structure

```
├── main.cc              # Core raytracing engine and physics calculations
├── Makefile             # Build system with optimization flags
├── black_hole_*.ppm     # Generated output images (800x600 resolution)
├── .gitignore           # Version control exclusions
└── README.md            # Project documentation
```

## Build and Execution

### Prerequisites
- **Compiler**: GCC 7+ or Clang 5+ with C++17 support
- **Libraries**: Standard C++ libraries only (no external dependencies)
- **System**: Linux/macOS/Windows with sufficient memory for image processing

### Compilation
```bash
# Basic build
make

# Clean build
make clean && make

# Run the renderer
./blackhole
```

### Output
The program generates PPM format images that can be converted to standard formats:
```bash
# Convert to PNG (requires ImageMagick)
convert black_hole_1.ppm black_hole.png

# View with any PPM-compatible viewer
# File size: ~3.5MB for 800x600 resolution
```

## Physics Details

### Schwarzschild Metric
The renderer implements the Schwarzschild solution to Einstein's field equations:
- **Event Horizon**: Located at r = 2GM/c²
- **Gravitational Field**: Inverse square law with relativistic corrections
- **Light Bending**: Calculated using geodesic equations

### Accretion Disk Model
- **Inner Radius**: 3 × Schwarzschild radius (innermost stable orbit)
- **Outer Radius**: 10 × Schwarzschild radius (outer disk boundary)
- **Temperature Gradient**: Decreasing temperature with distance from black hole
- **Color Mapping**: Temperature-based RGB values with contrast enhancement

## Code Architecture

### Core Classes
- **Vec3**: 3D vector with mathematical operations (dot product, cross product, normalization)
- **Color**: RGB color space with gamma correction and contrast enhancement
- **Camera**: Perspective camera with configurable parameters
- **BlackHole**: Physics object with gravitational field calculations

### Algorithm Flow
1. **Initialization**: Set up camera, black hole parameters, and output buffer
2. **Ray Generation**: Calculate ray directions for each pixel
3. **Physics Simulation**: Apply gravitational effects and calculate intersections
4. **Color Assignment**: Determine pixel colors based on physics calculations
5. **Post-Processing**: Apply gamma correction and contrast enhancement
6. **Output**: Write PPM format image to disk

## Performance Characteristics

- **Resolution**: 800×600 pixels (480,000 total rays)
- **Render Time**: 30-60 seconds on modern hardware
- **Memory Usage**: ~50MB for image buffer and calculations
- **Scalability**: Linear time complexity with pixel count

## Future Enhancements

### Physics Improvements
- [ ] Kerr metric for rotating black holes
- [ ] More accurate accretion disk physics
- [ ] Multiple black hole systems
- [ ] Gravitational wave effects

### Graphics Enhancements
- [ ] Anti-aliasing and supersampling
- [ ] Real-time rendering with OpenGL/Vulkan
- [ ] Advanced post-processing effects
- [ ] HDR rendering and tone mapping

### Performance Optimizations
- [ ] GPU acceleration with CUDA/OpenCL
- [ ] Multi-threading for CPU cores
- [ ] Adaptive resolution rendering
- [ ] Progressive refinement

## Technical Requirements

### Compiler Flags
```bash
# Recommended optimization flags
g++ -O3 -march=native -ffast-math -std=c++17 main.cc -o blackhole

# Debug build
g++ -g -O0 -std=c++17 main.cc -o blackhole_debug
```

### Memory Requirements
- **Minimum**: 100MB RAM
- **Recommended**: 1GB+ RAM for large images
- **Storage**: 10MB+ for output images

## Educational Value

This project demonstrates:
- **Advanced C++ Programming**: Classes, templates, operator overloading
- **Mathematical Programming**: Physics equations, 3D mathematics
- **Computer Graphics**: Raytracing, camera systems, image generation
- **Performance Optimization**: Algorithm efficiency, memory management
- **Scientific Computing**: Physics simulation, numerical methods

## License

MIT License - see LICENSE file for details.

## Author

Maxwell Corwin - maxwellcorwin13@gmail.com

