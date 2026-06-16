#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
private:
      vec3 origin;
      vec3 direction;
public:
      Ray(){}

      Ray(const vec3& origin,const vec3& direction)
            :origin(origin), direction(direction)
      { }

      //these getters return a const reference to the class object
      //so that the function doesn't make a copy which is cost consuming.
      const vec3& getOrigin() const { return origin;}
      const vec3& getDirection() const { return direction;}

      vec3 pointAt(double t) const {
            return origin + t*direction;
      }
};



#endif //RAY_H
