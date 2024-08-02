//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_SPHERE_H
#define RAYTRACER_SPHERE_H

#include <cmath>
#include "../hittable/hittable.h"
#include "../utils/common.h"
#include "../material/material.h"
#include "ray.h"
#include "vec3.h"
#include "aabb.h"

class Sphere : public hittable {
public:
    vec3 center;
    double radius;
    material mat;
    aabb box;

    Sphere(const vec3& center, double radius, const material& mat)
            : center(center), radius(radius), mat(mat) {
        vec3 radius_vec(radius, radius, radius);
        box = aabb(center - radius_vec, center + radius);
    }

    bool hit(const ray& r, const interval t, hit_record& rec) const override {
        vec3 oc = r.origin() - center;
        double a = r.direction().length_squared();
        double half_b = dot(oc, r.direction());
        double c = oc.length_squared() - radius * radius;

        double discriminant = half_b * half_b - a * c;

        if (discriminant < 0) {
            return false;
        }

        double sqrtd = std::sqrt(discriminant);
        double root = (-half_b - sqrtd) / a;

        if (root < t.min || t.max < root) {
            root = (-half_b + sqrtd) / a;
            if (root < t.min || t.max < root) {
                return false;
            }
        }

        rec.t = root;
        rec.p = r.at(root);
        vec3 outward_normal = (rec.p - (center)) / (radius);
        rec.set_face_normal(r, outward_normal);
        get_sphere_uv(outward_normal, rec);
        rec.mat = make_shared<material>(mat);

        return true;
    }

private:
    void get_sphere_uv(const vec3& normal, hit_record& rec) const {
        double theta = std::acos(-normal.y());
        double phi = std::atan2(-normal.z(), normal.x()) + pi;

        rec.u = phi / (2 * pi);
        rec.v = theta / pi;
    }

public:
    aabb bounding_box() const override {
        return box;
    }
};

#endif //RAYTRACER_SPHERE_H
