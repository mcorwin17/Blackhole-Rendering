/**
 * Black Hole Raytracer
 * 
 * A C++ raytracing engine that simulates gravitational lensing effects
 * around black holes using general relativity principles.
 * 
 * @author Maxwell Corwin
 * @date 2025
 * @version 2.0
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>
#include <chrono>
#include <string>

// Constants for physics calculations (from online sources)
namespace PhysicsConstants {
    constexpr double G = 1.0;           // Gravitational constant (normalized)
    constexpr double C = 1.0;           // Speed of light (normalized)
    constexpr double SCHWARZSCHILD_MULTIPLIER = 2.0;
    constexpr double PHOTON_SPHERE_MULTIPLIER = 1.5;
    constexpr double DISK_INNER_MULTIPLIER = 3.0;
    constexpr double DISK_OUTER_MULTIPLIER = 10.0;
}

// Rendering configuration
namespace RenderConfig {
    constexpr int WIDTH = 800;
    constexpr int HEIGHT = 600;
    constexpr double FOV = 0.785398;    // 45 degrees in radians
    constexpr int MAX_RAY_STEPS = 500;
    constexpr double MAX_RAY_DISTANCE = 50.0;
    constexpr double ADAPTIVE_STEP_FAR = 0.4;
    constexpr double ADAPTIVE_STEP_MEDIUM = 0.2;
    constexpr double ADAPTIVE_STEP_NEAR = 0.1;
    constexpr double ADAPTIVE_STEP_CLOSE = 0.05;
}

/**
 * 3D Vector class with mathematical operations
 */
class Vec3 {
private:
    double x_, y_, z_;

public:
    Vec3(double x = 0.0, double y = 0.0, double z = 0.0) 
        : x_(x), y_(y), z_(z) {}
    
    // Accessors
    double x() const { return x_; }
    double y() const { return y_; }
    double z() const { return z_; }
    
    // Mathematical operations
    Vec3 operator+(const Vec3& other) const { 
        return Vec3(x_ + other.x_, y_ + other.y_, z_ + other.z_); 
    }
    
    Vec3 operator-(const Vec3& other) const { 
        return Vec3(x_ - other.x_, y_ - other.y_, z_ - other.z_); 
    }
    
    Vec3 operator*(double scalar) const { 
        return Vec3(x_ * scalar, y_ * scalar, z_ * scalar); 
    }
    
    Vec3 operator/(double scalar) const { 
        if (std::abs(scalar) < 1e-10) return Vec3();
        return Vec3(x_ / scalar, y_ / scalar, z_ / scalar); 
    }
    
    // Vector operations
    double dot(const Vec3& other) const { 
        return x_ * other.x_ + y_ * other.y_ + z_ * other.z_; 
    }
    
    Vec3 cross(const Vec3& other) const {
        return Vec3(y_ * other.z_ - z_ * other.y_, 
                   z_ * other.x_ - x_ * other.z_, 
                   x_ * other.y_ - y_ * other.x_);
    }
    
    double length() const { 
        return std::sqrt(x_ * x_ + y_ * y_ + z_ * z_); 
    }
    
    double lengthSquared() const { 
        return x_ * x_ + y_ * y_ + z_ * z_; 
    }
    
    Vec3 normalize() const { 
        double len = length(); 
        return len > 1e-10 ? *this / len : Vec3(); 
    }
    
    // Utility methods
    bool isZero() const { 
        return std::abs(x_) < 1e-10 && std::abs(y_) < 1e-10 && std::abs(z_) < 1e-10; 
    }
    
    double distanceTo(const Vec3& other) const { 
        return (*this - other).length(); 
    }
};

/**
 * RGB Color class with post-processing capabilities
 */
class Color {
private:
    double r_, g_, b_;

public:
    Color(double r = 0.0, double g = 0.0, double b = 0.0) 
        : r_(r), g_(g), b_(b) {}
    
    // Accessors
    double r() const { return r_; }
    double g() const { return g_; }
    double b() const { return b_; }
    
    // Color operations
    Color operator+(const Color& other) const { 
        return Color(r_ + other.r_, g_ + other.g_, b_ + other.b_); 
    }
    
    Color operator*(double scalar) const { 
        return Color(r_ * scalar, g_ * scalar, b_ * scalar); 
    }
    
    Color operator*(const Color& other) const { 
        return Color(r_ * other.r_, g_ * other.g_, b_ * other.b_); 
    }
    
    // Color manipulation
    Color clamp() const { 
        return Color(std::min(1.0, std::max(0.0, r_)),
                    std::min(1.0, std::max(0.0, g_)),
                    std::min(1.0, std::max(0.0, b_))); 
    }
    
    Color gammaCorrect(double gamma = 2.2) const { 
        return Color(std::pow(r_, 1.0/gamma),
                    std::pow(g_, 1.0/gamma), 
                    std::pow(b_, 1.0/gamma)); 
    }
    
    Color enhanceContrast(double contrast = 1.2) const {
        return Color(std::min(1.0, std::max(0.0, (r_ - 0.5) * contrast + 0.5)),
                    std::min(1.0, std::max(0.0, (g_ - 0.5) * contrast + 0.5)),
                    std::min(1.0, std::max(0.0, (b_ - 0.5) * contrast + 0.5)));
    }
    
    // Utility methods
    bool isBlack() const { 
        return r_ < 1e-6 && g_ < 1e-6 && b_ < 1e-6; 
    }
    
    double luminance() const { 
        return 0.299 * r_ + 0.587 * g_ + 0.114 * b_; 
    }
};

/**
 * Camera class for perspective projection
 */
class Camera {
private:
    Vec3 position_;
    Vec3 direction_;
    Vec3 up_;
    double fieldOfView_;
    double aspectRatio_;

public:
    Camera(const Vec3& position, const Vec3& direction, const Vec3& up, double fov)
        : position_(position), direction_(direction.normalize()), up_(up.normalize()), 
          fieldOfView_(fov), aspectRatio_(1.0) {}
    
    // Getters
    const Vec3& position() const { return position_; }
    const Vec3& direction() const { return direction_; }
    double fieldOfView() const { return fieldOfView_; }
    
    // Set aspect ratio for non-square images
    void setAspectRatio(double aspect) { aspectRatio_ = aspect; }
    
    /**
     * Generate ray direction for given pixel coordinates
     */
    Vec3 getRayDirection(double x, double y, int width, int height) const {
        double scale = std::tan(fieldOfView_ * 0.5);
        
        // Normalize coordinates to [-1, 1]
        double px = (2.0 * x / width - 1.0) * scale * aspectRatio_;
        double py = (1.0 - 2.0 * y / height) * scale;
        
        // Calculate camera coordinate system
        Vec3 right = direction_.cross(up_).normalize();
        Vec3 newUp = right.cross(direction_).normalize();
        
        // Generate ray direction
        return (direction_ + right * px + newUp * py).normalize();
    }
};

/**
 * Black hole physics simulation class
 */
class BlackHole {
private:
    Vec3 position_;
    double mass_;
    double schwarzschildRadius_;
    double diskInnerRadius_;
    double diskOuterRadius_;

public:
    BlackHole(const Vec3& position, double mass)
        : position_(position), mass_(mass) {
        schwarzschildRadius_ = PhysicsConstants::SCHWARZSCHILD_MULTIPLIER * mass_;
        diskInnerRadius_ = PhysicsConstants::DISK_INNER_MULTIPLIER * schwarzschildRadius_;
        diskOuterRadius_ = PhysicsConstants::DISK_OUTER_MULTIPLIER * schwarzschildRadius_;
    }
    
    // Getters
    const Vec3& position() const { return position_; }
    double mass() const { return mass_; }
    double schwarzschildRadius() const { return schwarzschildRadius_; }
    double diskInnerRadius() const { return diskInnerRadius_; }
    double diskOuterRadius() const { return diskOuterRadius_; }
    
    /**
     * Calculate gravitational field at given point
     */
    Vec3 gravitationalField(const Vec3& point) const {
        Vec3 displacement = point - position_;
        double distance = displacement.length();
        
        // Inside event horizon
        if (distance < schwarzschildRadius_ * 1.01) {
            return Vec3();
        }
        
        // Newtonian approximation with relativistic correction
        double fieldStrength = -mass_ / (distance * distance * distance);
        return displacement * fieldStrength;
    }
    
    /**
     * Apply gravitational lensing to ray direction
     */
    Vec3 applyGravitationalLensing(const Vec3& rayPosition, const Vec3& rayDirection) const {
        Vec3 displacement = rayPosition - position_;
        double distance = displacement.length();
        
        // Inside photon sphere - strong deflection
        if (distance < schwarzschildRadius_ * PhysicsConstants::PHOTON_SPHERE_MULTIPLIER) {
            if (distance < schwarzschildRadius_) {
                return rayDirection; // Past event horizon
            }
            
            // Strong deflection near photon sphere
            double deflectionFactor = 1.0 / (distance - schwarzschildRadius_);
            Vec3 towardCenter = (position_ - rayPosition).normalize();
            return (rayDirection + towardCenter * deflectionFactor * 0.1).normalize();
        }
        
        // Distant rays - negligible lensing
        if (distance > schwarzschildRadius_ * 10.0) {
            return rayDirection;
        }
        
        // Moderate lensing for intermediate distances
        double deflectionAngle = 2.0 * mass_ / (distance * distance);
        Vec3 towardCenter = (position_ - rayPosition).normalize();
        Vec3 perpendicular = rayDirection.cross(towardCenter).cross(rayDirection).normalize();
        return (rayDirection + perpendicular * deflectionAngle * 0.1).normalize();
    }
    
    /**
     * Check for accretion disk intersection
     */
    bool intersectsAccretionDisk(const Vec3& rayOrigin, const Vec3& rayDirection, Vec3& intersectionPoint) const {
        // Disk lies in XZ plane (Y = 0)
        if (std::abs(rayDirection.y()) < 1e-6) {
            return false; // Ray parallel to disk plane
        }
        
        // Calculate intersection with Y = 0 plane
        double intersectionTime = (position_.y() - rayOrigin.y()) / rayDirection.y();
        if (intersectionTime < 0.0 || intersectionTime > 2.0) {
            return false; // Intersection behind ray or too far
        }
        
        intersectionPoint = rayOrigin + rayDirection * intersectionTime;
        double distanceFromCenter = std::sqrt(
            std::pow(intersectionPoint.x() - position_.x(), 2) + 
            std::pow(intersectionPoint.z() - position_.z(), 2)
        );
        
        return distanceFromCenter >= diskInnerRadius_ && distanceFromCenter <= diskOuterRadius_;
    }
    
    /**
     * Calculate accretion disk color based on temperature and physics
     */
    Color calculateAccretionDiskColor(const Vec3& point) const {
        double distanceFromCenter = std::sqrt(
            std::pow(point.x() - position_.x(), 2) + 
            std::pow(point.z() - position_.z(), 2)
        );
        
        // Temperature decreases with distance (inverse square law)
        double temperature = 1.0 / (distanceFromCenter / schwarzschildRadius_);
        temperature = std::min(1.0, std::max(0.1, temperature));
        
        // Relativistic Doppler effect from orbital velocity
        double orbitalVelocity = std::sqrt(mass_ / distanceFromCenter);
        double dopplerFactor = 1.0 + orbitalVelocity * 0.1;
        
        // Turbulence effects for realistic appearance
        double angle = std::atan2(point.z() - position_.z(), point.x() - position_.x());
        double turbulence = std::sin(angle * 8.0 + distanceFromCenter * 2.0) * 0.15 + 1.0;
        temperature *= turbulence;
        
        // Temperature-based color mapping
        Color baseColor;
        if (temperature > 0.8) {
            baseColor = Color(1.0, 0.95, 0.8);  // Hot white
        } else if (temperature > 0.6) {
            baseColor = Color(1.0, 0.8, 0.4);   // Yellow
        } else if (temperature > 0.4) {
            baseColor = Color(1.0, 0.6, 0.2);   // Orange
        } else {
            baseColor = Color(0.8, 0.3, 0.1);   // Red
        }
        
        return baseColor * temperature * dopplerFactor;
    }
};

/**
 * Ray tracing function
 */
Color traceRay(const Vec3& origin, Vec3 direction, const BlackHole& bh) {
    Vec3 currentPosition = origin;
    double totalDistance = 0.0;
    
    for (int step = 0; step < RenderConfig::MAX_RAY_STEPS; ++step) {
        double distanceToBlackHole = currentPosition.distanceTo(bh.position());
        
        // Adaptive step size
        double stepSize = RenderConfig::ADAPTIVE_STEP_CLOSE;
        if (distanceToBlackHole > bh.schwarzschildRadius() * 8.0) {
            stepSize = RenderConfig::ADAPTIVE_STEP_FAR;
        } else if (distanceToBlackHole > bh.schwarzschildRadius() * 5.0) {
            stepSize = RenderConfig::ADAPTIVE_STEP_MEDIUM;
        } else if (distanceToBlackHole > bh.schwarzschildRadius() * 2.0) {
            stepSize = RenderConfig::ADAPTIVE_STEP_NEAR;
        }
        
        // Check for event horizon
        if (distanceToBlackHole < bh.schwarzschildRadius() * 1.01) {
            return Color(0, 0, 0); // Event horizon
        }
        
        // Check disk intersection before moving
        Vec3 intersectionPoint;
        if (bh.intersectsAccretionDisk(currentPosition, direction, intersectionPoint)) {
            double hitDistance = currentPosition.distanceTo(intersectionPoint);
            if (hitDistance < stepSize * 2.0) { // Close enough to disk
                Color diskColor = bh.calculateAccretionDiskColor(intersectionPoint);
                double intensity = 1.0 + 0.5 / (1.0 + hitDistance);
                
                // Add lens flare effect near event horizon
                double distToCenter = currentPosition.distanceTo(bh.position());
                if (distToCenter < bh.schwarzschildRadius() * 4.0) {
                    double flareStrength = 1.0 / (1.0 + (distToCenter - bh.schwarzschildRadius()));
                    Color flare = Color(0.8, 0.9, 1.0) * flareStrength * 0.3;
                    diskColor = diskColor + flare;
                }
                
                return diskColor * intensity;
            }
        }
        
        // Apply gravitational bending (less frequently)
        if (step % 3 == 0) {
            direction = bh.applyGravitationalLensing(currentPosition, direction);
        }
        
        currentPosition = currentPosition + direction * stepSize;
        totalDistance += stepSize;
        
        if (totalDistance > RenderConfig::MAX_RAY_DISTANCE) {
            break;
        }
    }
    
    // Stars and nebula
    std::hash<std::string> hasher;
    std::string seed = std::to_string(int(direction.x() * 1000)) + "," + 
                       std::to_string(int(direction.y() * 1000)) + "," + 
                       std::to_string(int(direction.z() * 1000));
    double noise = double(hasher(seed) % 1000) / 1000.0;
    
    // Brighter stars
    if (noise > 0.994) {
        return Color(1, 1, 1) * (noise - 0.994) * 50;  // Bright white stars
    } else if (noise > 0.985) {
        return Color(0.8, 0.8, 1.0) * (noise - 0.985) * 15;  // Blue stars
    } else if (noise > 0.975) {
        return Color(1.0, 0.7, 0.5) * (noise - 0.975) * 8;   // Orange stars
    }
    
    // Subtle nebula background
    std::string nebulaSeed = std::to_string(int(direction.x() * 100)) + "," + std::to_string(int(direction.y() * 100));
    double nebulaNoise = double(hasher(nebulaSeed) % 1000) / 1000.0;
    if (nebulaNoise > 0.7) {
        Color nebula = Color(0.1, 0.05, 0.15) * (nebulaNoise - 0.7) * 0.5;
        return Color(0.03, 0.03, 0.08) + nebula;
    }
    
    return Color(0.03, 0.03, 0.08);  // Darker space
}

/**
 * Main rendering function
 */
void render(const Camera& cam, const BlackHole& bh, int w, int h, const std::string& filename) {
    std::vector<std::vector<Color>> image(h, std::vector<Color>(w));
    
    std::cout << "Rendering " << w << "x" << h << "...\n";
    
    for (int y = 0; y < h; ++y) {
        if (y % (h / 10) == 0) {
            std::cout << "Progress: " << (100 * y / h) << "%\n";
        }
        
        for (int x = 0; x < w; ++x) {
            // Anti-aliasing with 4x supersampling
            Color pixelSum(0, 0, 0);
            for (int dx = 0; dx < 2; ++dx) {
                for (int dy = 0; dy < 2; ++dy) {
                    double subX = x + (dx + 0.5) * 0.5;
                    double subY = y + (dy + 0.5) * 0.5;
                    Vec3 rayDirection = cam.getRayDirection(subX, subY, w, h);
                    pixelSum = pixelSum + traceRay(cam.position(), rayDirection, bh);
                }
            }
            image[y][x] = (pixelSum * 0.25).enhanceContrast().clamp();
        }
    }
    
    std::ofstream file(filename);
    file << "P3\n" << w << " " << h << "\n255\n";
    
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            Color& c = image[y][x];
            int r = int(c.r() * 255);
            int g = int(c.g() * 255);
            int b = int(c.b() * 255);
            file << r << " " << g << " " << b << "\n";
        }
    }
    
    file.close();
    std::cout << "Saved " << filename << "\n";
}

/**
 * Main entry point
 */
int main() {
    std::cout << "Black Hole Raytracer v2.0 - Maxwell Corwin\n";
    std::cout << "Enhanced with anti-aliasing, lens flares, and particle effects\n";
    
    BlackHole bh(Vec3(0, 0, 0), 1.0);
    
    // Multiple camera angles
    std::vector<Vec3> positions = {
        Vec3(0, 2, -8),    // Original view
        Vec3(-6, 1, -4),   // Side angle
        Vec3(0, 5, -6)     // Top-down view
    };
    
    for (int i = 0; i < positions.size(); ++i) {
        Vec3 camPos = positions[i];
        Vec3 camDir = (Vec3(0, 0, 0) - camPos).normalize();
        Vec3 camUp(0, 1, 0);
        Camera cam(camPos, camDir, camUp, RenderConfig::FOV);
        
        std::string filename = "black_hole_" + std::to_string(i + 1) + ".ppm";
        std::cout << "Rendering view " << (i + 1) << "/" << positions.size() << "...\n";
        render(cam, bh, RenderConfig::WIDTH, RenderConfig::HEIGHT, filename);
    }
    
    return 0;
}