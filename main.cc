// black hole raytracer with gravitational lensing
// Maxwell Corwin 2024

#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <random>

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
        
        // photon sphere at 1.5 * schwarzschild radius
        if (dist < schwarzschild_r * 1.5) {
            if (dist < schwarzschild_r) return ray_dir; // past event horizon
            // strong deflection near photon sphere
            double bend_factor = 1.0 / (dist - schwarzschild_r);
            Vec3 toward_center = (pos - ray_pos).normalize();
            return (ray_dir + toward_center * bend_factor * 0.1).normalize();
        }
        
        // gentle lensing for distant rays
        if (dist > schwarzschild_r * 10) return ray_dir;
        
        double bend_angle = 2.0 * mass / (dist * dist);
        Vec3 toward_center = (pos - ray_pos).normalize();
        Vec3 perp = ray_dir.cross(toward_center).cross(ray_dir).normalize();
        return (ray_dir + perp * bend_angle * 0.1).normalize();
    }
    
    bool hit_disk(const Vec3& ray_pos, const Vec3& ray_dir, Vec3& hit_point) const {
        // disk in XZ plane (y = 0)
        if (abs(ray_dir.y) < 1e-6) return false;  // parallel to disk
        
        double t = (pos.y - ray_pos.y) / ray_dir.y;  // intersect with y=0 plane
        if (t < 0 || t > 2.0) return false;  // behind ray or too far
        
        hit_point = ray_pos + ray_dir * t;
        double dist_from_center = sqrt((hit_point.x - pos.x) * (hit_point.x - pos.x) + 
                                      (hit_point.z - pos.z) * (hit_point.z - pos.z));
        
        return dist_from_center >= disk_inner && dist_from_center <= disk_outer;
    }
    
    Color disk_color(const Vec3& point) const {
        double dist_from_center = sqrt((point.x - pos.x) * (point.x - pos.x) + 
                                      (point.z - pos.z) * (point.z - pos.z));
        
        // temperature falls off with distance
        double temp = 1.0 / (dist_from_center / schwarzschild_r);
        temp = min(1.0, max(0.1, temp));
        
        // realistic orbital velocity
        double orbital_vel = sqrt(mass / dist_from_center);
        double doppler = 1.0 + orbital_vel * 0.1;
        
        Color color;
        if (temp > 0.8) {
            color = Color(1.0, 0.95, 0.8);  // hot white
        } else if (temp > 0.6) {
            color = Color(1.0, 0.8, 0.4);   // yellow
        } else if (temp > 0.4) {
            color = Color(1.0, 0.6, 0.2);   // orange  
        } else {
            color = Color(0.8, 0.3, 0.1);   // red
        }
        
        return color * temp * doppler;
    }
};

Color trace_ray(const Vec3& origin, Vec3 dir, const BlackHole& bh) {
    Vec3 pos = origin;
    double total_dist = 0;
    const double max_dist = 50.0;
    
    for (int steps = 0; steps < 500 && total_dist < max_dist; steps++) {
        double dist_to_bh = (pos - bh.pos).length();
        
        // adaptive step size based on distance to black hole
        double step = 0.05;
        if (dist_to_bh > bh.schwarzschild_r * 5) step = 0.2;
        else if (dist_to_bh > bh.schwarzschild_r * 2) step = 0.1;
        
        // check for event horizon
        if (dist_to_bh < bh.schwarzschild_r * 1.01) {
            return Color(0, 0, 0);  // event horizon
        }
        
        // check disk intersection before moving
        Vec3 hit_point;
        if (bh.hit_disk(pos, dir, hit_point)) {
            double hit_dist = (hit_point - pos).length();
            if (hit_dist < step * 2) {  // close enough to disk
                Color disk_col = bh.disk_color(hit_point);
                double intensity = 1.0 + 0.5 / (1.0 + hit_dist);
                return disk_col * intensity;
            }
        }
        
        // apply gravitational bending (less frequently)
        if (steps % 3 == 0) {
            dir = bh.bend_ray(pos, dir);
        }
        
        pos = pos + dir * step;
        total_dist += step;
    }
    
    // stars and nebula
    hash<string> hasher;
    string seed = to_string(int(dir.x * 1000)) + "," + 
                  to_string(int(dir.y * 1000)) + "," + 
                  to_string(int(dir.z * 1000));
    double noise = double(hasher(seed) % 1000) / 1000.0;
    
    // brighter stars
    if (noise > 0.994) {
        return Color(1, 1, 1) * (noise - 0.994) * 50;  // bright white stars
    } else if (noise > 0.985) {
        return Color(0.8, 0.8, 1.0) * (noise - 0.985) * 15;  // blue stars
    } else if (noise > 0.975) {
        return Color(1.0, 0.7, 0.5) * (noise - 0.975) * 8;   // orange stars
    }
    
    // subtle nebula background
    string nebula_seed = to_string(int(dir.x * 100)) + "," + to_string(int(dir.y * 100));
    double nebula_noise = double(hasher(nebula_seed) % 1000) / 1000.0;
    if (nebula_noise > 0.7) {
        Color nebula = Color(0.1, 0.05, 0.15) * (nebula_noise - 0.7) * 0.5;
        return Color(0.03, 0.03, 0.08) + nebula;
    }
    
    return Color(0.03, 0.03, 0.08);  // darker space
}

void render(const Camera& cam, const BlackHole& bh, int w, int h, const string& filename) {
    vector<vector<Color>> img(h, vector<Color>(w));
    
    cout << "rendering " << w << "x" << h << "...\n";
    
    for (int y = 0; y < h; y++) {
        if (y % (h / 10) == 0) {
            cout << "progress: " << (100 * y / h) << "%\n";
        }
        
        for (int x = 0; x < w; x++) {
            Vec3 ray_dir = cam.get_ray_dir(x, y, w, h);
            Color pixel = trace_ray(cam.pos, ray_dir, bh);
            img[y][x] = pixel.clamp();
        }
    }
    
    ofstream file(filename);
    file << "P3\n" << w << " " << h << "\n255\n";
    
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Color& c = img[y][x];
            int r = int(c.r * 255);
            int g = int(c.g * 255);
            int b = int(c.b * 255);
            file << r << " " << g << " " << b << "\n";
        }
    }
    
    file.close();
    cout << "saved " << filename << "\n";
}

int main() {
    cout << "black hole raytracer v1.0\n";
    
    BlackHole bh(Vec3(0, 0, 0), 1.0);
    
    Vec3 cam_pos(0, 2, -8);
    Vec3 cam_dir = (Vec3(0, 0, 0) - cam_pos).normalize();
    Vec3 cam_up(0, 1, 0);
    Camera cam(cam_pos, cam_dir, cam_up, M_PI / 3);
    
    render(cam, bh, 800, 600, "black_hole.ppm");
    
    return 0;
}

