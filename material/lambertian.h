//
// Created by Hanson Siu on 27/07/2024.
//

#ifndef RAYTRACER_LAMBERTIAN_H
#define RAYTRACER_LAMBERTIAN_H

#include "material.h"


class lambertian : public material {
public:
    lambertian(const colour& albedo) : tex(make_shared<solid_colour>(albedo)) {}
    lambertian(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        srec.attenuation = tex->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr     = make_shared<cosine_pdf>(rec.normal);
        srec.skip_pdf    = false;
        return true;
    }

    double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const override {
        auto cos_theta = dot(rec.normal, unit_vector(scattered.direction()));
        return cos_theta < 0 ? 0 : cos_theta / pi;
    }

private:
    shared_ptr<texture> tex;
};


#endif //RAYTRACER_LAMBERTIAN_H
