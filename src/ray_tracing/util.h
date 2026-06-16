#ifndef UTIL_H
#define UTIL_H

#include <memory>
#include <fstream>
#include <iostream>
#include <cmath>
#include <limits>

using namespace std;
using std::make_shared;
using std::shared_ptr;

// Constants

extern ofstream image_file;
constexpr double infinity = std::numeric_limits<double>::infinity();
constexpr double pi = 3.1415926535897932385;

// Utility Functions

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double() {
    // returns a random double in the range [0,1)
    // rand()/RAND_MAX return [0,1], the maximum returned value would be 1
    // so if we add RAND_MAX + 1.0 would have: RAND_MAX/(RAND_MAX + 1.0) which is < 1
    return rand()/(RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    // returns a random double in the range [min,max)
    // example: min = 10, max = 20, random_double = 10+10*(0.2)
    return min + (max-min)*random_double();
}

// linear to gamma conversion: exponent of 1/gammma (using gamma = 2)
inline double linear_to_gamma(double linear_component) {
    if (linear_component > 0)
        return sqrt(linear_component);
    return 0;
}


#endif //UTIL_H
