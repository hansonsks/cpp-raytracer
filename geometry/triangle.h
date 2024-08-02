//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_TRIANGLE_H
#define RAYTRACER_TRIANGLE_H

#include <cmath>
#include <limits>
#include <array>
#include "../material/material.h"
#include "vec2.h"
#include "vec3.h"
#include "ray.h"
#include "../hittable/hittable.h"
#include "aabb.h"
#include "../utils/common.h"

class triangle : public hittable {
public:
    std::array<vec3, 3> vertexes;
    std::array<vec2, 3> uvVertex;
    vec3 e1, e2, normal;
    material mat;

    triangle(const std::array<vec3, 3>& vertexes, const material& material) : vertexes(vertexes) {
        e1 = vertexes[1] - vertexes[0];
        e2 = vertexes[2] - vertexes[0];
        mat = material;
        normal = unit_vector(cross(e1, e2));
    }

    triangle(const std::array<vec3, 3>& vertexes, const std::array<vec2, 3>& uvData, material m)
            : vertexes(vertexes), uvVertex(uvData) {
        e1 = vertexes[1] - vertexes[0];
        e2 = vertexes[2] - vertexes[0];
        mat = m;
        normal = unit_vector(cross(e1, e2));
    }

    triangle(const vec3& v1, const vec3& v2, const vec3& v3, const std::array<vec2, 3>& uvData, material m)
            : uvVertex(uvData) {
        vertexes = {v1, v2, v3};
        e1 = vertexes[1] - vertexes[0];
        e2 = vertexes[2] - vertexes[0];
        mat = m;
        normal = unit_vector(cross(e1, e2));
    }

private:
    vec2 calculateObjTextureUV(double u, double v) const {
        return (uvVertex[2] * u) + (uvVertex[1] * v) + (uvVertex[0] * (1 - u - v));
    }

public:
    void convertToObjUV(hit_record& rec) const {
        if (!uvVertex.empty()) {
            vec2 uv = calculateObjTextureUV(rec.u, rec.v);
            rec.u = uv.x();
            rec.v = uv.y();
        }
    }

    bool hit(const ray& r, interval it, hit_record& rec) const override {
        if (std::fabs(dot(normal, r.direction())) < 1e-8) return false;

        vec3 s0 = r.origin() - vertexes[0];
        vec3 s1 = cross(r.direction(), e2);
        vec3 s2 = cross(s0, e1);

        double s1e1 = dot(s1, e1);
        double t = dot(s2, e2) / s1e1;

        if (t < it.min || t > it.max) return false;

        double b1 = dot(s1, s0) / s1e1;
        if (b1 > 0) {
            double b2 = dot(s2, r.direction()) / s1e1;
            if (b2 > 0 && (1 - b1 - b2) > 0) {
                rec.t = t;
                rec.u = b2;
                rec.v = b1;
                convertToObjUV(rec);
                rec.p = r.at(t);
                rec.set_face_normal(r, normal);
                rec.mat = make_shared<material>(mat);

                return true;
            }
        }
        return false;
    }

    aabb bounding_box() const override {
        vec3 min = vec3(infinity, infinity, infinity);
        vec3 max = vec3(-infinity, -infinity, -infinity);

        for (const auto& vertex : vertexes) {
            min = vec3(std::min(min.x(), vertex.x()),
                       std::min(min.y(), vertex.y()),
                       std::min(min.z(), vertex.z()));
            max = vec3(std::max(max.x(), vertex.x()),
                       std::max(max.y(), vertex.y()),
                       std::max(max.z(), vertex.z()));
        }

        return aabb(min - vec3(1e-8, 1e-8, 1e-8), max + vec3(1e-8, 1e-8, 1e-8));
    }
};

#endif //RAYTRACER_TRIANGLE_H
