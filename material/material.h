//
// Created by Hanson Siu on 27/07/2024.
//

#ifndef RAYTRACER_MATERIAL_H
#define RAYTRACER_MATERIAL_H

#include "../utils/common.h"
#include "../texture/texture.h"


class hit_record;

class material {
public:
    virtual ~material() = default;

    virtual colour emitted(double u, double v, const point3& p) const {
        return colour(0,0,0);
    }

    virtual bool scatter(
            const ray& r_in, const hit_record& rec, colour& attenuation, ray& scattered
    ) const {
        return false;
    }
};

#endif //RAYTRACER_MATERIAL_H
