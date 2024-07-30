//
// Created by Hanson Siu on 27/07/2024.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "../utils/common.h"
#include "../utils/pdf.h"
#include "../texture/texture.h"


class hit_record;

class scatter_record {
public:
    colour attenuation;
    shared_ptr<pdf> pdf_ptr;
    bool skip_pdf;
    ray skip_pdf_ray;
};


class material {
public:
    virtual ~material() = default;

    virtual colour emitted(const ray& r_in, const hit_record& rec, double u, double v, const point3& p) const {
        return {0, 0, 0};
    }

    virtual bool scatter(const ray& r_in, const hit_record& rec, scatter_record& srec) const {
        return false;
    }

    virtual double scattering_pdf(const ray& r_in, const hit_record& rec, const ray& scattered) const {
        return 0;
    }
};

#endif //RAYTRACER_MATERIAL_H
