//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_NOISE_TEXTURE_H
#define RAYTRACER_NOISE_TEXTURE_H

#include "texture.h"


class noise_texture : public texture {
public:
    noise_texture(double scale) : scale(scale) {}

    colour value(double u, double v, const point3& p) const override {
        return colour(.5, .5, .5) * (1 + std::sin(scale * p.z() + 10 * noise.turb(p, 7)));
    }

private:
    perlin noise;
    double scale;
};

#endif //RAYTRACER_NOISE_TEXTURE_H
