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
    
    Color clamp() const { 
        return Color(min(1.0, max(0.0, r)),
                    min(1.0, max(0.0, g)),
                    min(1.0, max(0.0, b))); 
    }
};

struct Camera {
    Vec3 pos, dir, up;
    double fov;
    
    Camera(Vec3 p, Vec3 d, Vec3 u, double f) : pos(p), dir(d.normalize()), up(u.normalize()), fov(f) {}
    
    Vec3 get_ray_dir(double x, double y, int w, int h) const {
        double aspect = double(w) / h;
        double scale = tan(fov * 0.5);
        
        double px = (2 * x / w - 1) * scale * aspect;
        double py = (1 - 2 * y / h) * scale;
        
        Vec3 right = dir.cross(up).normalize();
        Vec3 new_up = right.cross(dir).normalize();
        
        return (dir + right * px + new_up * py).normalize();
    }
};

int main() {
    cout << "camera test\n";
    
    Camera cam(Vec3(0, 0, -5), Vec3(0, 0, 1), Vec3(0, 1, 0), M_PI / 3);
    
    Vec3 ray = cam.get_ray_dir(400, 300, 800, 600);
    cout << "center ray: " << ray.x << " " << ray.y << " " << ray.z << "\n";
    
    return 0;
}