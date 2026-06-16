#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.h"
#include "Hittable.h"

class Material {
    private:
public:
    ~Material() = default;
    // produced a scattered ray (or say it absorbed the ray)
    // if scattered, how much the ray is attenuated
    virtual bool scatter(const Ray& ray, const hit_record& record, vec3& attenuation, Ray& scattered) const {
        return false;
    }
};

class lambertian : public Material {
private:
    vec3 albedo; //whiteness
public:
    lambertian(const vec3& albedo) : albedo(albedo) {

    }
    //in the lambertian model we choose to always scatter and also the attenuation doesn't change based whether we the ray scattered
    bool scatter(const Ray& ray, const hit_record& record, vec3& attenuation, Ray &scattered) const override {
        //we must compute the reflected vector(direction) from the ray hit
        //that will be (Lambertian diffuse reflection): Center = normal + hit_point,
        //S = random_unit_vector + Center = random_unit_vector + normal + hit_point
        //S-hit_point = random_unit_vector + normal + hit_point - hit_point (S-hit_point=reflection)
        //direction = random_unit_vector + normal      (S is a point in the radius of a unit_sphere that intersects with our hit_point)
        vec3 direction = record.normal + random_unit_vec3();
        if (direction.close_to_zero()) {
            direction = record.normal;
        }
        scattered = Ray(record.hit_point, direction);
        attenuation = albedo;
        return true;
    }
};

class Metal : public Material {
private:
    vec3 albedo;
    double fuzz;
public:
    Metal(const vec3& albedo, double fuzz) : albedo(albedo), fuzz(fuzz<1?fuzz:1) {

    }

    bool scatter(const Ray& ray, const hit_record& record, vec3& attenuation, Ray &scattered) const override {
        vec3 direction = reflect(ray.getDirection(), record.normal);
        direction = unit_vector(direction) + fuzz * random_unit_vec3();
        scattered = Ray(record.hit_point, direction);
        attenuation = albedo;
        return dot(scattered.getDirection(), record.normal) > 0.0;
    }
};

class dielectric : public Material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const Ray& ray, const hit_record& record, vec3& attenuation, Ray& scattered)
    const override {
        attenuation = vec3(1.0, 1.0, 1.0);
        //if the ray hit the front_face we must apply a change to the ray(1/refraction_index)
        double ri = record.front_face ?1.0/refraction_index : refraction_index;

        vec3 unit_direction = unit_vector(ray.getDirection());
        double cos_theta = fmin(dot(-1*unit_direction, record.normal), 1.0);
        double sin_theta = sqrt(1.0-cos_theta*cos_theta);

        bool cannot_refract = sin_theta*ri > 1;
        vec3 direction;
        if (cannot_refract) {
            direction = reflect(unit_direction, record.normal); //if the etha(η/η') value is greater than 1 then we have no solution for refraction,
                                                                    //so we reflect the ray
        }
        else {
            direction = refract(unit_direction, record.normal, ri); //refract
        }

        scattered = Ray(record.hit_point, direction);
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over
    // the refractive index of the enclosing media
    double refraction_index;
};


#endif //MATERIAL_H
