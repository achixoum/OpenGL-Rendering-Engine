#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <ostream>
#include "util.h"

using namespace std;

class vec3 {
private:
    double x;
    double y;
    double z;
public:
    vec3() {
        x = 0;
        y = 0;
        z = 0;
    }
    vec3(double x, double y, double z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    double getX() const { return x;}

    double getY() const { return y;}

    double getZ() const { return z;}

    vec3& add(vec3& b) {
        this->x += b.getX();
        this->y += b.getY();
        this->z += b.getZ();
        return *this;
    }

    vec3& sub(vec3& b) {
        this->x -= b.getX();
        this->y -= b.getY();
        this->z -= b.getZ();
        return *this;
    }

    double length() const {
        return sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
    }

    double length_squared() const {
        return this->x * this->x + this->y * this->y + this->z * this->z;
    }

    vec3& operator +=(const vec3& b) {
        this->x += b.x;
        this->y += b.y;
        this->z += b.z;
        return *this;
    }

    vec3& operator -=(vec3& b) {
        this->x -= b.x;
        this->y -= b.y;
        this->z -= b.z;
        return *this;
    }

    vec3& operator *=(double c) {
        this->x *= c;
        this->y *= c;
        this->z *= c;
        return *this;
    }

    vec3& operator /=(double c) {
        this->x /= c;
        this->y /= c;
        this->z /= c;
        return *this;
    }

    static vec3 random_vec3() {
        return vec3(random_double(), random_double(), random_double());
    }

    static vec3 random_vec3(double min, double max) {
        return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

    bool close_to_zero() const{
        double s = 1e-8;
        return fabs(x)<s && fabs(y)<s && fabs(z)<s;
    }
};

inline vec3 operator+(const vec3& a, const vec3& b) {
    return vec3(a.getX() + b.getX(), a.getY() + b.getY(), a.getZ() + b.getZ());
}

inline vec3 operator-(const vec3& a, const vec3& b) {
    return vec3(a.getX() - b.getX(), a.getY() - b.getY(), a.getZ() - b.getZ());
}


inline vec3 operator*(double c, const vec3& a) {
    return vec3(a.getX()*c, a.getY()*c, a.getZ()*c);
}

inline vec3 operator*(const vec3& a, double c) {
    return vec3(a.getX()*c, a.getY()*c, a.getZ()*c);
}

inline vec3 operator*(const vec3& a, const vec3& b) {
    return vec3(a.getX()*b.getX(), a.getY()*b.getY(), a.getZ()*b.getZ());
}

inline vec3 operator/(const vec3& a, double c) {
    return (1.0/c)*a;
}

inline ostream& operator<<(ostream& out, const vec3& v) {
    return out <<"("<< v.getX() << ' ' << v.getY() << ' ' << v.getZ()<<")"<<endl;
}

inline double dot(const vec3& a, const vec3& b) {
    return a.getX()*b.getX() + a.getY()*b.getY() + a.getZ()*b.getZ();
}

inline vec3 cross(const vec3& a, const vec3& b) {
    return vec3(a.getY() * b.getZ() - a.getZ() * b.getY(),a.getZ() * b.getX() - a.getX() * b.getZ(),a.getX() * b.getY() - a.getY() * b.getX());
}

inline vec3 unit_vector(const vec3& a) {
    return a/a.length();
}

inline vec3 random_unit_vec3() {
    while (true) {
        vec3 point = vec3::random_vec3(-1,1);
        double lensqr = point.length_squared();
        if (1e-160 < lensqr && lensqr <= 1) {
            return point/sqrt(lensqr);
        }
    }
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 unit_on_sphere = random_unit_vec3();
    if (dot(unit_on_sphere, normal) > 0.0) {      // is on the right hemisphere(with the normal)
        return unit_on_sphere;
    }
    return unit_on_sphere*-1;
}

// v is the direction of a ray intersection with a normal n
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& ray_direction, const vec3& normal, double etai_over_etat) {
    double cos_theta = fmin(dot(-1*ray_direction, normal),1.0);
    vec3 R_perpendicular = etai_over_etat*(ray_direction + cos_theta*normal);
    vec3 R_paraller = -sqrt(fabs(1.0 - R_perpendicular.length_squared())) * normal;
    return R_paraller + R_perpendicular;
}

inline vec3 random_in_unit_disk() {
    while (true) {
        vec3 point = vec3(random_double(-1,1), random_double(-1,1),0);
        if (point.length_squared() < 1)
            return point;
    }
}

#endif //VEC3_H
