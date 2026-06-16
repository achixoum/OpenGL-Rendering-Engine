#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "Hittable.h"
#include <vector>
#include <memory>
#include "Interval.h"

using namespace std;

class Hittable_list : public Hittable{
private:
    vector<shared_ptr<Hittable>> objects;
public:
    Hittable_list();
    Hittable_list(const shared_ptr<Hittable>& object);
    void clear();
    void add_object(const shared_ptr<Hittable>& object);
    bool hit(const Ray &ray, Interval ray_interval, hit_record &hit_rec) const override;
};



#endif //HITTABLE_LIST_H
