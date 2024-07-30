//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_IMAGE_TEXTURE_H
#define RAYTRACER_IMAGE_TEXTURE_H

#include "texture.h"


class image_texture : public texture {
public:
    image_texture(const char* filename) : image(filename) {}

    colour value(double u, double v, const point3& p) const override {
        // If we have no texture data, then return solid cyan as a debugging aid
        if (image.height() <= 0) {
            return {0,1,1};
        }

        // Clamp input texture coordinates to [0,1] x [1,0]
        u = interval(0,1).clamp(u);

        // Flip V to image coordinates
        v = 1.0 - interval(0,1).clamp(v);

        auto i = int(u * image.width());
        auto j = int(v * image.height());
        auto pixel = image.pixel_data(i,j);

        auto colour_scale = 1.0 / 255.0;
        return {colour_scale * pixel[0], colour_scale * pixel[1], colour_scale * pixel[2]};
    }

private:
    rtw_image image;
};

#endif //RAYTRACER_IMAGE_TEXTURE_H
