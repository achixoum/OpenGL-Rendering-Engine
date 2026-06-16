#include "Hittable_list.h"

Hittable_list::Hittable_list() {
    objects = vector<shared_ptr<Hittable>>();
}

void Hittable_list::add_object(const shared_ptr<Hittable>& object) {
    objects.push_back(object);
}

Hittable_list::Hittable_list(const shared_ptr<Hittable>& object) {
    add_object(object);
}

void Hittable_list::clear() {
    objects.clear();
}

bool Hittable_list::hit(const Ray &ray, Interval ray_interval, hit_record &hit_rec) const {
    hit_record temp_rec;
    double closest_so_far = ray_interval.getMax();
    bool hit_anything = false;

    for (const auto& object: objects) {
        if (object->hit(ray,Interval(ray_interval.getMin(), closest_so_far), temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            hit_rec = temp_rec;
        }
    }

    return hit_anything;
}

