#ifndef INTERVAL_H
#define INTERVAL_H

#include "util.h"

class Interval {
private:
    double min, max;
public:
    Interval() : min(infinity) , max(-infinity) {}

    Interval(double min, double max) {
        this->min = min;
        this->max = max;
    }

    // xε[min,max]
    bool contains(double x) const{
        return x>=min && x<=max;
    }

    // xε(min, max)
    bool surrounds(double x) const {
        return x>min && x<max;
    }

    double clamp(double x) const {
        if (x<min)
            return min;
        if (x>max)
            return max;
        return x;
    }

    double getMin() const { return min;}
    double getMax() const { return max;}

    static const Interval empty, universe;
};

#endif //INTERVAL_H
