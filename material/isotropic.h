//
// Created by Hanson Siu on 27/07/2024.
//

#ifndef RAYTRACER_ISOTROPIC_H
#define RAYTRACER_ISOTROPIC_H

#include "material.h"


class isotropic : public material {
public:
    isotropic(const colour& albedo) : tex(make_shared<solid_colour>(albedo)) {}
    isotropic(shared_ptr<texture> tex) : tex(tex) {}

    bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const override {
        srec.attenuation = tex->value(rec.u, rec.v, rec.p);
        srec.pdf_ptr = make_shared<sphere_pdf>();
        srec.skip_pdf = false;
        return true;
    }

    double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const override {
        return 1 / (4 * pi);
    }

private:
    shared_ptr<texture> tex;
};


#endif //RAYTRACER_ISOTROPIC_H
