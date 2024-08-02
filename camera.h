//
// Created by Hanson Siu on 28/07/2024.
//

#ifndef RAYTRACER_CAMERA_H
#define RAYTRACER_CAMERA_H

#include "hittable/hittable.h"
#include "material/material.h"
#include "utils/common.h"
#include "utils/pdf.h"


class camera {
public:
    double aspect_ratio      = 1.0;  // Ratio of image width over height
    int    img_width         = 100;  // Rendered image width in pixel count
    int    samples_per_pixel = 10;   // Count of random samples for each pixel
    int    max_depth         = 10;   // Maximum number of ray bounces into scene
    colour  background;              // Scene background colour

    double vfov     = 90;                             // Vertical FOV
    point3 look_from = point3(0, 0, 0);   // point camera is looking from
    point3 look_at   = point3(0, 0, -1);  // point camera is looking at
    vec3   vup      = vec3(0,1,0);        // Camera-relative "up" direction

    double defocus_angle = 0;  // Variation angle of rays through each pixel
    double focus_dist = 10;    // Distance from camera look_from point to plane of perfect focus

    void render(const hittable& world, const hittable& lights) {
        initialize();

        std::cout << "P3\n" << img_width << ' ' << img_height << "\n255\n";

        for (int j = 0; j < img_height; j++) {
            std::clog << "\rScanlines remaining: " << (img_height - j) << ' ' << std::flush;
            for (int i = 0; i < img_width; i++) {
                colour pixel_colour(0,0,0);
                for (int s_j = 0; s_j < sqrt_spp; s_j++) {
                    for (int s_i = 0; s_i < sqrt_spp; s_i++) {
                        ray r = get_ray(i, j, s_i, s_j);
                        pixel_colour += ray_colour(r, max_depth, world, lights);
                        // std::clog << pixel_colour.to_string() << std::flush;
                    }
                }

                write_colour(std::cout, pixel_samples_scale * pixel_colour);
            }
        }

        std::clog << "\rDone.                 \n";
    }

private:
    int    img_height;           // Rendered image height
    double pixel_samples_scale;  // colour scale factor for a sum of pixel samples
    int    sqrt_spp;             // Square root of number of samples per pixel
    double recip_sqrt_spp;       // 1 / sqrt_spp
    point3 center;               // Camera center
    point3 pixel00_loc;          // Location of pixel 0, 0
    vec3   pixel_delta_u;        // Offset to pixel to the right
    vec3   pixel_delta_v;        // Offset to pixel below
    vec3   u, v, w;              // Camera frame basis vectors
    vec3   defocus_disk_u;       // Defocus disk horizontal radius
    vec3   defocus_disk_v;       // Defocus disk vertical radius

    void initialize() {
        img_height = int(img_width / aspect_ratio);
        img_height = (img_height < 1) ? 1 : img_height;

        sqrt_spp = int(std::sqrt(samples_per_pixel));
        pixel_samples_scale = 1.0 / (sqrt_spp * sqrt_spp);
        recip_sqrt_spp = 1.0 / sqrt_spp;

        center = look_from;

        // Determine viewport dimensions
        auto theta = degrees_to_radians(vfov);
        auto h = std::tan(theta/2);
        auto viewport_height = 2 * h * focus_dist;
        auto viewport_width = viewport_height * (double(img_width) / img_height);

        // Calculate the u, v, w unit basis vectors for the camera coordinate frame
        w = unit_vector(look_from - look_at);
        u = unit_vector(cross(vup, w));
        v = cross(w, u);

        // Calculate the vectors across the horizontal and down the vertical viewport edges
        vec3 viewport_u = viewport_width * u;    // Vector across viewport horizontal edge
        vec3 viewport_v = viewport_height * -v;  // Vector down viewport vertical edge

        // Calculate the horizontal and vertical delta vectors from pixel to pixel
        pixel_delta_u = viewport_u / img_width;
        pixel_delta_v = viewport_v / img_height;

        // Calculate the location of the upper left pixel
        auto viewport_upper_left = center - (focus_dist * w) - viewport_u / 2 - viewport_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

        // Calculate the camera defocus disk basis vectors
        auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
        defocus_disk_u = u * defocus_radius;
        defocus_disk_v = v * defocus_radius;
    }

    // Construct a camera ray originating from the defocus disk and directed at a randomly
    // sampled point around the pixel location i, j for stratified sample square s_i, s_j.
    ray get_ray(int i, int j, int s_i, int s_j) const {
        auto offset = sample_square_stratified(s_i, s_j);
        auto pixel_sample = pixel00_loc
                            + ((i + offset.x()) * pixel_delta_u)
                            + ((j + offset.y()) * pixel_delta_v);

        auto ray_origin = (defocus_angle <= 0) ? center : defocus_disk_sample();
        auto ray_direction = pixel_sample - ray_origin;
        auto ray_time = random_double();

        // std::clog << ray_origin.to_string() + " " + ray_direction.to_string() + " " + std::to_string(ray_time) << std::flush;
        return {ray_origin, ray_direction, ray_time};
    }

    // Returns the vector to a random point in the square sub-pixel specified by grid
    // indices s_i and s_j, for an idealized unit square pixel [-.5,-.5] to [+.5,+.5]
    vec3 sample_square_stratified(int s_i, int s_j) const {
        auto px = ((s_i + random_double()) * recip_sqrt_spp) - 0.5;
        auto py = ((s_j + random_double()) * recip_sqrt_spp) - 0.5;
        return {px, py, 0};
    }

    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square
    vec3 sample_square() const {
        return {random_double() - 0.5, random_double() - 0.5, 0};
    }

    // Returns a random point in the unit (radius 0.5) disk centered at the origin
    vec3 sample_disk(double radius) const { return radius * random_in_unit_disk(); }

    // Returns a random point in the camera defocus disk
    point3 defocus_disk_sample() const {
        auto p = random_in_unit_disk();
        return center + (p[0] * defocus_disk_u) + (p[1] * defocus_disk_v);
    }

    // If we've exceeded the ray bounce limit, no more light is gathered
    colour ray_colour(const ray& r, int depth, const hittable& world, const hittable& lights) const {
        if (depth <= 0) {
            return {0, 0, 0};
        }

        hit_record rec;

        // If the ray hits nothing, return the background colour
        if (!world.hit(r, interval(0.001, infinity), rec)) {
            return background;
        }

        scatter_record srec;
        colour colour_from_emission = rec.mat->emitted(r, rec, rec.u, rec.v, rec.p);

        if (!rec.mat->scatter(r, rec, srec)) {
            return colour_from_emission;
        }

        if (srec.skip_pdf) {
            return srec.attenuation * ray_colour(srec.skip_pdf_ray, depth - 1, world, lights);
        }

        auto light_ptr = make_shared<hittable_pdf>(lights, rec.p);
        mixture_pdf p(light_ptr, srec.pdf_ptr);

        ray scattered = ray(rec.p, p.generate(), r.time());
        auto pdf_value = p.value(scattered.direction());

        double scattering_pdf = rec.mat->scattering_pdf(r, rec, scattered);

        colour sample_colour = ray_colour(scattered, depth-1, world, lights);
        colour colour_from_scatter = (srec.attenuation * scattering_pdf * sample_colour) / pdf_value;

        return colour_from_emission + colour_from_scatter;
    }
};

#endif //RAYTRACER_CAMERA_H
