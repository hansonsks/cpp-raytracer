//
// Created by Hanson Siu on 25/07/2024.
//

#ifndef RAYTRACER_RAY_H
#define RAYTRACER_RAY_H

#include "vec3.h"


class ray {
public:
    ray() = default;
    ray(const point3& origin, const vec3& direction) : orig(origin), dir(direction) {}

    const point3& origin() const  { return orig; }
    const vec3& direction() const { return dir; }
    point3 at(double t) const { return orig + t*dir; }

private:
    point3 orig;
    vec3 dir;
};

#endif //RAYTRACER_RAY_H
