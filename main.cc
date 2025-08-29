// basic vector math for raytracing
// Maxwell Corwin 2024

#include <iostream>
#include <cmath>

using namespace std;

struct Vec3 {
    double x, y, z;
    
    Vec3(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z) {}
    
    Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
    Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
    Vec3 operator*(double t) const { return Vec3(x * t, y * t, z * t); }
    Vec3 operator/(double t) const { return Vec3(x / t, y / t, z / t); }
    
    double dot(const Vec3& v) const { return x * v.x + y * v.y + z * v.z; }
    Vec3 cross(const Vec3& v) const {
        return Vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
    }
    
    double length() const { return sqrt(x * x + y * y + z * z); }
    Vec3 normalize() const { double l = length(); return l > 0 ? *this / l : Vec3(); }
};

struct Color {
    double r, g, b;
    
    Color(double r = 0, double g = 0, double b = 0) : r(r), g(g), b(b) {}
    
    Color operator+(const Color& c) const { 
        return Color(r + c.r, g + c.g, b + c.b); 
    }
    Color operator*(double t) const { 
        return Color(r * t, g * t, b * t); 
    }
};

int main() {
    cout << "vector math test\n";
    
    Vec3 a(1, 0, 0);
    Vec3 b(0, 1, 0);
    Vec3 c = a.cross(b);
    
    cout << "cross product: " << c.x << " " << c.y << " " << c.z << "\n";
    
    return 0;
}