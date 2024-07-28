//
// Created by Hanson Siu on 27/07/2024.
//

#ifndef RAYTRACER_METAL_H
#define RAYTRACER_METAL_H

#include "material.h"


class metal : public material {
public:
    metal(const colour& albedo, double fuzz) : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered)
    const override {
        vec3 reflected = reflect(r_in.direction(), rec.normal);
        reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
        scattered = ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }

private:
    colour albedo;
    double fuzz;
};

#endif //RAYTRACER_METAL_H
