//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_SOLID_COLOUR_H
#define RAYTRACER_SOLID_COLOUR_H

#include "texture.h"


class solid_colour : public texture {
public:
    solid_colour(const colour& albedo) : albedo(albedo) {}

    solid_colour(double red, double green, double blue) : solid_colour(colour(red,green,blue)) {}

    colour value(double u, double v, const point3& p) const override { return albedo; }

private:
    colour albedo;
};


#endif //RAYTRACER_SOLID_COLOUR_H
