#include "RTSphere.h"

#include <utility>


RTSphere::RTSphere (const double radius, const vec3& center, shared_ptr<Material> material) : radius(fmax(radius, 0)), center(center), material_(material) {
}

bool RTSphere::hit(const Ray &ray, Interval ray_interval, hit_record &hit_rec) const{
    vec3 CQ = center - ray.getOrigin();
    double a = ray.getDirection().length_squared();
    double h = dot(ray.getDirection(), CQ);
    double c = CQ.length_squared() - radius*radius;

    double discriminant = h*h - a*c;

    if (discriminant < 0.0) {
        return false;
    }

    double d_sqrt = sqrt(discriminant);

    // Find the nearest root that lies in the acceptable range(t_min, t_max)
    double root = (h - d_sqrt)/a;
    if (!ray_interval.surrounds(root)) {
        root = (h + d_sqrt)/a;
        if (!ray_interval.surrounds(root)) {
            return false;
        }
    }

    hit_rec.t = root;
    hit_rec.material_type = material_;
    hit_rec.hit_point = ray.pointAt(hit_rec.t);
    hit_rec.normal = (hit_rec.hit_point - center)/radius;    // sphere normals can be made unit length simply divide by the radius.
    hit_rec.set_face_normal(ray, hit_rec.normal);

    return true;
}
