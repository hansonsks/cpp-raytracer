//
// Created by Hanson Siu on 28/07/2024.
//

#ifndef RAYTRACER_COMMON_H
#define RAYTRACER_COMMON_H

#include <chrono>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <memory>
#include <random>

using std::make_shared;
using std::shared_ptr;


const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double degrees_to_radians(double degrees) {
    return degrees * pi / 180.0;
}

// Returns a random real in [0,1)
inline double random_double() {
    static std::mt19937 generator(std::chrono::system_clock::now().time_since_epoch().count());
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    return distribution(generator);
}

// Returns a random real in [min,max)
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

// Returns a random integer in [min,max]
inline int random_int(int min, int max) {
    return int(random_double(min, max+1));
}


#include "../geometry/colour.h"
#include "interval.h"
#include "../geometry/ray.h"
#include "../geometry/vec3.h"

#endif //RAYTRACER_COMMON_H
