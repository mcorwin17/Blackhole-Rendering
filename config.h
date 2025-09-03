/**
 * @file config.h
 * @brief Configuration parameters for Black Hole Raytracer
 * @author Maxwell Corwin
 * @date 2025
 * @version 2.0
 * 
 * Centralized configuration for all rendering, physics, and system parameters.
 * This allows easy tuning of the simulation without modifying core code.
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <string>

namespace Config {
    
    // =========================================================================
    // Physics Constants
    // =========================================================================
    namespace Physics {
        constexpr double GRAVITATIONAL_CONSTANT = 1.0;           // Normalized G
        constexpr double SPEED_OF_LIGHT = 1.0;                   // Normalized c
        constexpr double SCHWARZSCHILD_MULTIPLIER = 2.0;         // Event horizon
        constexpr double PHOTON_SPHERE_MULTIPLIER = 1.5;         // Photon sphere
        constexpr double DISK_INNER_MULTIPLIER = 3.0;            // Innermost stable orbit
        constexpr double DISK_OUTER_MULTIPLIER = 10.0;           // Outer disk boundary
        constexpr double LENSING_STRENGTH = 0.1;                 // Gravitational lensing intensity
        constexpr double TURBULENCE_FREQUENCY = 8.0;             // Disk turbulence frequency
        constexpr double TURBULENCE_AMPLITUDE = 0.15;            // Turbulence effect strength
    }
    
    // =========================================================================
    // Rendering Configuration
    // =========================================================================
    namespace Rendering {
        constexpr int DEFAULT_WIDTH = 800;                        // Default image width
        constexpr int DEFAULT_HEIGHT = 600;                       // Default image height
        constexpr double FIELD_OF_VIEW = 0.785398;               // 45 degrees in radians
        constexpr int MAX_RAY_STEPS = 500;                        // Maximum ray marching steps
        constexpr double MAX_RAY_DISTANCE = 50.0;                 // Maximum ray travel distance
        constexpr int SUPERSAMPLING_LEVEL = 2;                    // Anti-aliasing samples (2x2)
        constexpr bool ENABLE_ANTI_ALIASING = true;               // Enable supersampling
        constexpr bool ENABLE_POST_PROCESSING = true;              // Enable color enhancement
    }
    
    // =========================================================================
    // Performance Tuning
    // =========================================================================
    namespace Performance {
        constexpr double ADAPTIVE_STEP_FAR = 0.4;                 // Step size for distant rays
        constexpr double ADAPTIVE_STEP_MEDIUM = 0.2;              // Step size for medium distance
        constexpr double ADAPTIVE_STEP_NEAR = 0.1;                // Step size for near rays
        constexpr double ADAPTIVE_STEP_CLOSE = 0.05;              // Step size for close rays
        constexpr int GRAVITATIONAL_UPDATE_FREQUENCY = 3;         // Update lensing every N steps
        constexpr double DISK_INTERSECTION_THRESHOLD = 2.0;       // Disk hit detection threshold
    }
    
    // =========================================================================
    // Visual Effects
    // =========================================================================
    namespace Effects {
        constexpr bool ENABLE_LENS_FLARE = true;                  // Enable lens flare effects
        constexpr bool ENABLE_DOPPLER_SHIFT = true;               // Enable relativistic Doppler
        constexpr bool ENABLE_TURBULENCE = true;                  // Enable disk turbulence
        constexpr double LENS_FLARE_INTENSITY = 0.3;              // Lens flare brightness
        constexpr double DOPPLER_AMPLITUDE = 0.1;                 // Doppler effect strength
        constexpr double STAR_BRIGHTNESS_MULTIPLIER = 50.0;       // Bright star intensity
        constexpr double NEBULA_VISIBILITY_THRESHOLD = 0.7;       // Nebula visibility threshold
    }
    
    // =========================================================================
    // Color and Post-Processing
    // =========================================================================
    namespace PostProcessing {
        constexpr double GAMMA_CORRECTION = 2.2;                  // Standard gamma correction
        constexpr double CONTRAST_ENHANCEMENT = 1.2;              // Contrast boost factor
        constexpr double TEMPERATURE_MIN = 0.1;                   // Minimum temperature for coloring
        constexpr double TEMPERATURE_MAX = 1.0;                   // Maximum temperature for coloring
        constexpr bool ENABLE_COLOR_CLAMPING = true;              // Clamp colors to valid range
    }
    
    // =========================================================================
    // Camera Presets
    // =========================================================================
    namespace CameraPresets {
        struct CameraConfig {
            std::string name;
            double x, y, z;                                       // Position
            double targetX, targetY, targetZ;                     // Look-at point
            double upX, upY, upZ;                                 // Up vector
        };
        
        // Predefined camera angles for different views
        constexpr CameraConfig VIEWS[] = {
            {"Front View",     0.0,  2.0, -8.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0},
            {"Side View",     -6.0,  1.0, -4.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0},
            {"Top View",       0.0,  5.0, -6.0,  0.0, 0.0, 0.0,  0.0, 0.0, -1.0},
            {"Close View",     0.0,  1.0, -4.0,  0.0, 0.0, 0.0,  0.0, 1.0, 0.0},
            {"Wide View",      0.0,  3.0, -12.0, 0.0, 0.0, 0.0,  0.0, 1.0, 0.0}
        };
        
        constexpr size_t NUM_VIEWS = sizeof(VIEWS) / sizeof(VIEWS[0]);
    }
    
    // =========================================================================
    // Output Configuration
    // =========================================================================
    namespace Output {
        constexpr std::string OUTPUT_FORMAT = "PPM";              // Output image format
        constexpr std::string FILENAME_PREFIX = "black_hole_";    // Output file prefix
        constexpr bool GENERATE_MULTIPLE_VIEWS = true;            // Generate all camera views
        constexpr bool SHOW_PROGRESS = true;                      // Display rendering progress
        constexpr bool VERBOSE_OUTPUT = false;                    // Detailed logging
    }
    
    // =========================================================================
    // System Configuration
    // =========================================================================
    namespace System {
        constexpr bool ENABLE_MULTITHREADING = false;             // Enable parallel processing
        constexpr int MAX_THREADS = 8;                            // Maximum thread count
        constexpr bool ENABLE_MEMORY_OPTIMIZATION = true;         // Memory usage optimization
        constexpr size_t MAX_MEMORY_USAGE = 1024 * 1024 * 1024;  // 1GB memory limit
    }
    
    // =========================================================================
    // Validation Functions
    // =========================================================================
    namespace Validation {
        bool validateConfiguration();
        void printConfiguration();
        void exportConfiguration(const std::string& filename);
    }
    
} // namespace Config

#endif // CONFIG_H
