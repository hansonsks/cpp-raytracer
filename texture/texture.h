//
// Created by Hanson Siu on 28/07/2024.
//

#ifndef RAYTRACER_TEXTURE_H
#define RAYTRACER_TEXTURE_H

#include <utility>

#include "../utils/common.h"
#include "perlin.h"
#include "../rtw_stb_image.h"


class texture {
public:
    virtual ~texture() = default;

    virtual colour value(double u, double v, const point3& p) const = 0;
};

#endif //RAYTRACER_TEXTURE_H
