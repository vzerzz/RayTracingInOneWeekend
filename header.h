#pragma once

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>

// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions

inline double deg2rad(double degrees) {
    return degrees * pi / 180.0;
}

inline double random_double(){
    //return a random real in [0,1)
    return rand()/(RAND_MAX + 1.0);//1.0 ... 不能是1 不然输出唯一
}

inline double random_double(double min, double max){
    //return a random real in [min,max)
    return min + (max - min)*random_double();
}

inline double clamp(double x, double min, double max){
    if(x > max) return max;
    if(x < min) return min;
    return x;
}

// Common Headers

#include "ray.h"
#include "vec.h"