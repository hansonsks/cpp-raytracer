//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_MESH_3D_MODEL_H
#define RAYTRACER_MESH_3D_MODEL_H

#include <vector>
#include <stdexcept>
#include <cmath>

#include "../hittable/hittable.h"
#include "aabb.h"
#include "ray.h"
#include "../utils/interval.h"
#include "vec2.h"
#include "triangle.h"



class mesh_3d_model : public hittable {
public:
    std::vector<std::shared_ptr<hittable>> TrianglesData; // Use shared_ptr for memory management
    aabb box;

    mesh_3d_model() : box() {}

    mesh_3d_model(const std::vector<std::shared_ptr<hittable>>& data) : TrianglesData(data), box() {
        for (const auto& obj : data) {
            box = aabb(box, obj->bounding_box());
        }
    }

    void add(std::shared_ptr<triangle> object) {
        TrianglesData.push_back(object);
        box = aabb(box, object->bounding_box());
    }

    bool hit(const ray& r, interval t, hit_record& rec) const override {
        bool hit_anything = false;
        for (const auto& object : TrianglesData) {
            if (object->hit(r, t, rec)) {
                t = interval(t.min, rec.t);
                hit_anything = true;
            }
        }
        return hit_anything;
    }

    aabb bounding_box() const override {
        return box;
    }

    void scale(double s) {
        box = aabb();
        for (size_t i = 0; i < TrianglesData.size(); ++i) {
            if (auto* tri = dynamic_cast<triangle*>(TrianglesData[i].get())) {
                std::array<vec3, 3> new_vertexes = {
                        tri->vertexes[0] * s,
                        tri->vertexes[1] * s,
                        tri->vertexes[2] * s
                };
                TrianglesData[i] = std::make_shared<triangle>(new_vertexes, tri->uvVertex, tri->mat);
                box = aabb(box, TrianglesData[i]->bounding_box());
            }
        }
    }

    void mirror(int axis) {
        box = aabb(); // Reset the bounding box
        for (size_t i = 0; i < TrianglesData.size(); ++i) {
            if (auto* tri = dynamic_cast<triangle*>(TrianglesData[i].get())) {
                std::array<vec3, 3> new_vertexes;

                switch (axis) {
                    case 0: // Mirror along x-axis
                        new_vertexes = {
                                vec3(-tri->vertexes[0].x(), tri->vertexes[0].y(), tri->vertexes[0].z()),
                                vec3(-tri->vertexes[1].x(), tri->vertexes[1].y(), tri->vertexes[1].z()),
                                vec3(-tri->vertexes[2].x(), tri->vertexes[2].y(), tri->vertexes[2].z()),
                        };
                        break;

                    case 1: // Mirror along y-axis
                        new_vertexes = {
                                vec3(tri->vertexes[0].x(), -tri->vertexes[0].y(), tri->vertexes[0].z()),
                                vec3(tri->vertexes[1].x(), -tri->vertexes[1].y(), tri->vertexes[1].z()),
                                vec3(tri->vertexes[2].x(), -tri->vertexes[2].y(), tri->vertexes[2].z()),
                        };
                        break;

                    case 2: // Mirror along z-axis
                        new_vertexes = {
                                vec3(tri->vertexes[0].x(), tri->vertexes[0].y(), -tri->vertexes[0].z()),
                                vec3(tri->vertexes[1].x(), tri->vertexes[1].y(), -tri->vertexes[1].z()),
                                vec3(tri->vertexes[2].x(), tri->vertexes[2].y(), -tri->vertexes[2].z()),
                        };
                        break;
                }

                TrianglesData[i] = std::make_shared<triangle>(new_vertexes, tri->uvVertex, tri->mat);
                box = aabb(box, TrianglesData[i]->bounding_box());
            }
        }
    }

    void translate(double x, double y, double z) {
        box = aabb();
        vec3 move_vec(x, y, z);
        for (size_t i = 0; i < TrianglesData.size(); ++i) {
            if (triangle* tri = dynamic_cast<triangle*>(TrianglesData[i].get())) {
                tri->vertexes[0] += move_vec;
                tri->vertexes[1] += move_vec;
                tri->vertexes[2] += move_vec;
                box = aabb(box, TrianglesData[i]->bounding_box());
            }
        }
    }

    void rotate(double angle, int axis) {
        box = aabb();
        double radians = degrees_to_radians(angle);
        double sin_theta = sin(radians);
        double cos_theta = cos(radians);

        vec3 max_p(box.x.max, box.y.max, box.z.max);
        vec3 min_p(box.x.min, box.y.min, box.z.min);
        vec3 model_distance_size = max_p - min_p;
        vec3 model_mid = model_distance_size / 2;

        for (size_t i = 0; i < TrianglesData.size(); ++i) {
            if (triangle* tri = dynamic_cast<triangle*>(TrianglesData[i].get())) {
                std::array<vec3, 3> new_vertexes = {
                        tri->vertexes[0],
                        tri->vertexes[1],
                        tri->vertexes[2]
                };

                for (int j = 0; j < 3; ++j) {
                    tri->vertexes[j] -= (min_p + model_mid);
                }

                switch (axis) {
                    case 0: // Rotate around x-axis
                        new_vertexes[0].set_y(tri->vertexes[0].y() * cos_theta - tri->vertexes[0].z() * sin_theta);
                        new_vertexes[0].set_z(tri->vertexes[0].z() * cos_theta + tri->vertexes[0].y() * sin_theta);
                        new_vertexes[1].set_y(tri->vertexes[1].y() * cos_theta - tri->vertexes[1].z() * sin_theta);
                        new_vertexes[1].set_z(tri->vertexes[1].z() * cos_theta + tri->vertexes[1].y() * sin_theta);
                        new_vertexes[2].set_y(tri->vertexes[2].y() * cos_theta - tri->vertexes[2].z() * sin_theta);
                        new_vertexes[2].set_z(tri->vertexes[2].z() * cos_theta + tri->vertexes[2].y() * sin_theta);
                        break;

                    case 1: // Rotate around y-axis
                        new_vertexes[0].set_x(tri->vertexes[0].x() * cos_theta - tri->vertexes[0].z() * sin_theta);
                        new_vertexes[0].set_z(tri->vertexes[0].z() * cos_theta + tri->vertexes[0].x() * sin_theta);
                        new_vertexes[1].set_x(tri->vertexes[1].x() * cos_theta - tri->vertexes[1].z() * sin_theta);
                        new_vertexes[1].set_z(tri->vertexes[1].z() * cos_theta + tri->vertexes[1].x() * sin_theta);
                        new_vertexes[2].set_x(tri->vertexes[2].x() * cos_theta - tri->vertexes[2].z() * sin_theta);
                        new_vertexes[2].set_z(tri->vertexes[2].z() * cos_theta + tri->vertexes[2].x() * sin_theta);
                        break;

                    case 2: // Rotate around z-axis
                        new_vertexes[0].set_x(tri->vertexes[0].x() * cos_theta - tri->vertexes[0].y() * sin_theta);
                        new_vertexes[0].set_y(tri->vertexes[0].y() * cos_theta + tri->vertexes[0].x() * sin_theta);
                        new_vertexes[1].set_x(tri->vertexes[1].x() * cos_theta - tri->vertexes[1].y() * sin_theta);
                        new_vertexes[1].set_y(tri->vertexes[1].y() * cos_theta + tri->vertexes[1].x() * sin_theta);
                        new_vertexes[2].set_x(tri->vertexes[2].x() * cos_theta - tri->vertexes[2].y() * sin_theta);
                        new_vertexes[2].set_y(tri->vertexes[2].y() * cos_theta + tri->vertexes[2].x() * sin_theta);
                        break;
                }

                for (int j = 0; j < 3; ++j) {
                    new_vertexes[j] += (min_p + model_mid);
                }

                TrianglesData[i] = std::make_shared<triangle>(new_vertexes, tri->uvVertex, tri->mat);
                box = aabb(box, TrianglesData[i]->bounding_box());
            }
        }
    }
};

#endif //RAYTRACER_MESH_3D_MODEL_H
