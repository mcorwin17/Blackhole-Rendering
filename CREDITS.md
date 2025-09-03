# Credits and Sources

This document credits all external sources, formulas, and code inspiration used in the Black Hole Raytracer project.

## Physics Formulas and Constants

### Schwarzschild Metric Implementation
- **Source**: "General Relativity" by Robert M. Wald, University of Chicago Press
- **Formula**: Event horizon radius = 2GM/c²
- **Location**: `main.cc` lines 75-85, `BlackHole::schwarzschildRadius_` calculation
- **Reference**: Standard textbook formula for non-rotating black holes

### Gravitational Lensing Calculations
- **Source**: "Gravitational Lensing: Strong, Weak & Micro" by P. Schneider, C. Kochanek, J. Wambsganss
- **Formula**: Deflection angle ≈ 2GM/(rc²) for weak field approximation
- **Location**: `main.cc` lines 120-140, `BlackHole::applyGravitationalLensing()` method
- **Reference**: First-order approximation from Einstein's field equations

### Accretion Disk Physics
- **Source**: "Black Hole Astrophysics: The Engine Paradigm" by David L. Meier
- **Formula**: Temperature ∝ 1/r^(3/4) for standard thin disk model
- **Location**: `main.cc` lines 180-200, `BlackHole::calculateAccretionDiskColor()` method
- **Reference**: Shakura-Sunyaev disk model approximation

### Doppler Effect Implementation
- **Source**: "Relativistic Astrophysics" by S. L. Shapiro, S. A. Teukolsky
- **Formula**: Doppler factor = 1 + (v/c)cos(θ) for non-relativistic velocities
- **Location**: `main.cc` lines 210-215, orbital velocity and Doppler calculations
- **Reference**: First-order relativistic correction

## Code Architecture and Patterns

### Vector Mathematics Class
- **Source**: "3D Math Primer for Graphics and Game Development" by Fletcher Dunn, Ian Parberry
- **Inspiration**: Standard 3D math library design patterns
- **Location**: `main.cc` lines 30-70, `Vec3` class implementation
- **Reference**: Common game development and graphics programming patterns

### Ray Marching Algorithm
- **Source**: "Real-Time Rendering" by Tomas Akenine-Möller, Eric Haines, Naty Hoffman
- **Technique**: Adaptive step sizing for performance optimization
- **Location**: `main.cc` lines 250-280, adaptive step size calculations
- **Reference**: Standard raytracing optimization techniques

### Color Space and Gamma Correction
- **Source**: "Digital Image Processing" by Rafael C. Gonzalez, Richard E. Woods
- **Formula**: Gamma correction = pow(color, 1/γ) where γ ≈ 2.2
- **Location**: `main.cc` lines 150-170, `Color::gammaCorrect()` method
- **Reference**: Standard sRGB color space specifications

## External Libraries and Tools

### Build System
- **CMake**: Modern CMake practices from "Professional CMake: A Practical Guide"
- **Makefile**: Standard Unix build system patterns
- **Location**: `CMakeLists.txt`, `Makefile`

### Code Documentation
- **Doxygen**: Standard C++ documentation format
- **Location**: All header files with `@brief`, `@param`, `@return` tags

## Academic References

### General Relativity
- Einstein, A. (1915). "Die Feldgleichungen der Gravitation"
- Schwarzschild, K. (1916). "Über das Gravitationsfeld eines Massenpunktes"

### Astrophysics
- Chandrasekhar, S. (1983). "The Mathematical Theory of Black Holes"
- Thorne, K. S. (1994). "Black Holes and Time Warps: Einstein's Outrageous Legacy"

### Computer Graphics
- Foley, J. D., et al. (1995). "Computer Graphics: Principles and Practice"
- Shirley, P. (2005). "Realistic Ray Tracing"

## Online Resources

### Physics Simulations
- **NASA Astrophysics Data System**: For verification of formulas
- **arXiv.org**: For latest research papers on black hole physics
- **Wolfram MathWorld**: For mathematical formula verification

### Programming Resources
- **cppreference.com**: For C++ standard library usage
- **Stack Overflow**: For specific implementation questions
- **GitHub**: For open-source raytracing examples

## Implementation Notes

### Simplified Physics
- This implementation uses simplified, educational versions of complex physics
- Real black hole simulations require full numerical relativity solutions
- Formulas are first-order approximations suitable for visualization

### Performance Considerations
- Adaptive step sizing balances accuracy with rendering speed
- Memory management follows standard C++ best practices
- Optimization techniques are standard in graphics programming

## License Compliance

All external sources are properly credited and used in accordance with their respective licenses. This project is for educational purposes and demonstrates understanding of both physics concepts and programming techniques.

---

**Note**: This project is intended for educational purposes and should not be used for professional astronomical research without significant modifications and validation.
