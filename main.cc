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

class BlackHole {
public:
    Vec3 pos;
    double mass;
    double schwarzschild_r;
    double disk_inner, disk_outer;
    
    BlackHole(Vec3 p, double m) : pos(p), mass(m) {
        schwarzschild_r = 2.0 * mass;
        disk_inner = 3.0 * schwarzschild_r;
        disk_outer = 10.0 * schwarzschild_r;
    }
    
    Vec3 gravity(const Vec3& point) const {
        Vec3 r = point - pos;
        double dist = r.length();
        if (dist < schwarzschild_r * 1.01) return Vec3();
        
        double factor = -mass / (dist * dist * dist);
        return r * factor;
    }
    
    Vec3 bend_ray(const Vec3& ray_pos, const Vec3& ray_dir) const {
        Vec3 r = ray_pos - pos;
        double dist = r.length();
        
        if (dist < schwarzschild_r * 1.1) return ray_dir;
        
        double bend_angle = 4.0 * mass / dist;
        Vec3 perp = ray_dir.cross(r).cross(ray_dir).normalize();
        return (ray_dir + perp * bend_angle).normalize();
    }
    
    bool hit_disk(const Vec3& ray_pos, const Vec3& ray_dir, Vec3& hit_point) const {
        if (abs(ray_dir.y) < 1e-6) return false;
        
        double t = -ray_pos.y / ray_dir.y;
        if (t < 0) return false;
        
        hit_point = ray_pos + ray_dir * t;
        double dist = (hit_point - pos).length();
        
        return dist >= disk_inner && dist <= disk_outer;
    }
    
    Color disk_color(const Vec3& point) const {
        Vec3 r = point - pos;
        double dist = r.length();
        
        double temp = 1.0 / (dist / schwarzschild_r);
        temp = min(1.0, max(0.1, temp));
        
        if (temp > 0.8) {
            return Color(1.0, 0.9, 0.6) * temp;  // hot
        } else if (temp > 0.5) {
            return Color(1.0, 0.6, 0.2) * temp;  // warm
        } else {
            return Color(0.8, 0.2, 0.1) * temp;  // cool
        }
    }
};

Color trace_ray(const Vec3& origin, Vec3 dir, const BlackHole& bh) {
    double step = 0.1;
    double max_dist = 20.0;
    Vec3 pos = origin;
    
    for (double t = 0; t < max_dist; t += step) {
        dir = bh.bend_ray(pos, dir);
        pos = pos + dir * step;
        
        double dist_to_bh = (pos - bh.pos).length();
        
        if (dist_to_bh < bh.schwarzschild_r) {
            return Color(0, 0, 0);  // fell into black hole
        }
        
        Vec3 hit_point;
        if (bh.hit_disk(pos, dir, hit_point)) {
            return bh.disk_color(hit_point);
        }
    }
    
    return Color(0.02, 0.02, 0.05); // space background
}

int main() {
    cout << "ray marching test\n";
    
    BlackHole bh(Vec3(0, 0, 0), 1.0);
    Vec3 ray_origin(0, 0, -5);
    Vec3 ray_dir(0, 0, 1);
    
    Color result = trace_ray(ray_origin, ray_dir, bh);
    cout << "ray result: " << result.r << " " << result.g << " " << result.b << "\n";
    
    return 0;
}
