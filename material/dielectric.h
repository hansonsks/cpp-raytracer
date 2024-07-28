//
// Created by Hanson Siu on 27/07/2024.
//

#ifndef RAYTRACER_DIELECTRIC_H
#define RAYTRACER_DIELECTRIC_H

#include "material.h"


class dielectric : public material {
public:
    dielectric(double refraction_index) : refraction_index(refraction_index) {}

    bool scatter(const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered) const override {
        attenuation = colour(1.0, 1.0, 1.0);
        double ri = rec.front_face ? (1.0 / refraction_index) : refraction_index;

        vec3 unit_direction = unit_vector(r_in.direction());
        double cos_theta    = std::fmin(dot(-unit_direction, rec.normal), 1.0);
        double sin_theta    = std::sqrt(1.0 - (cos_theta * cos_theta));

        bool cannot_refract = ri * sin_theta > 1.0;

        vec3 direction;
        if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
            direction = reflect(unit_direction, rec.normal);
        } else {
            direction = refract(unit_direction, rec.normal, ri);
        }

        scattered = ray(rec.p, direction, r_in.time());
        return true;
    }

private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over the enclosing media
    double refraction_index;

    // Use Schlick's approximation for reflectance
    static double reflectance(double cosine, double refraction_index) {
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0 * r0;
        return r0 + (1 - r0) * std::pow((1 - cosine),5);
    }
};

#endif //RAYTRACER_DIELECTRIC_H
