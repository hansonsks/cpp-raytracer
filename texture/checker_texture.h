//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_CHECKER_TEXTURE_H
#define RAYTRACER_CHECKER_TEXTURE_H

#include "texture.h"


class checker_texture : public texture {
public:
    checker_texture(double scale, shared_ptr<texture> even, shared_ptr<texture> odd)
            : inv_scale(1.0 / scale), even(std::move(even)), odd(std::move(odd)) {}

    checker_texture(double scale, const colour& c1, const colour& c2)
            : checker_texture(scale,
                              make_shared<solid_colour>(c1),
                              make_shared<solid_colour>(c2)) {}

    colour value(double u, double v, const point3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.x()));
        auto yInteger = int(std::floor(inv_scale * p.y()));
        auto zInteger = int(std::floor(inv_scale * p.z()));

        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

private:
    double inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

#endif //RAYTRACER_CHECKER_TEXTURE_H
