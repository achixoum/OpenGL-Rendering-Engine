#ifndef RTSPHERE_H
#define RTSPHERE_H

#include "Hittable.h"
#include <iostream>
#include <cmath>

#include "Material.h"

using namespace std;

class RTSphere : public Hittable {
private:
    double radius;
    vec3 center;
    shared_ptr<Material> material_;
public:
    RTSphere (double radius, const vec3& center, shared_ptr<Material> material);
    bool hit(const Ray &ray, Interval ray_interval, hit_record &hit_rec) const override;
};



#endif //RTSPHERE_H
