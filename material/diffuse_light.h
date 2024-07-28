//
// Created by Hanson Siu on 27/07/2024.
//

#ifndef RAYTRACER_DIFFUSE_LIGHT_H
#define RAYTRACER_DIFFUSE_LIGHT_H

#include "material.h"


class diffuse_light : public material {
public:
    diffuse_light(shared_ptr<texture> tex) : tex(tex) {}
    diffuse_light(const colour& emit) : tex(make_shared<solid_colour>(emit)) {}

    colour emitted(double u, double v, const point3& p) const override {
        return tex->value(u, v, p);
    }

private:
    shared_ptr<texture> tex;
};

#endif //RAYTRACER_DIFFUSE_LIGHT_H
