#ifndef HITTABLE_H
#define HITTABLE_H

#include "Ray.h"
#include "Interval.h"

class Material;

//represents the information we need when a ray hits an object in our world
//the intersection point (hit_point) between the ray and the object,
//from which side the ray hit the object(front_face)
//the normal vector of the object that was produced by the ray and the hit_point (the normal is perpendicular to the objects surface)
//and the parameter "t" that shows in which point our ray line(P(t) = origin + t*direction) hit the object's surface
// also the material type that we hit(if it is metal then the ray reflects other objects)
typedef struct {
    vec3 hit_point;
    vec3 normal;    //must be unit vector
    double t;
    bool front_face;
    shared_ptr<Material> material_type;

    void set_face_normal(const Ray& ray, vec3& outward_normal) {
        //NOTE: the `outward_normal` is assumed to be a unit vector

        front_face = dot(ray.getDirection(), outward_normal) < 0.0;
        normal = front_face ? outward_normal : outward_normal*-1;
    }
}hit_record;


//represents the objects that can intersect with rays
class Hittable {
public:
    virtual ~Hittable() = default;
    virtual bool hit(const Ray& ray, Interval ray_interval, hit_record& hit_rec) const = 0;
};



#endif //HITTABLE_H
