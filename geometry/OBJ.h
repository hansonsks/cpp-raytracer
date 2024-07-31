//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_OBJ_H
#define RAYTRACER_OBJ_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <string>

#include "../material/material.h"
#include "mesh_3d_model.h"


class OBJ {
public:
    static mesh_3d_model load_to_mesh_3d_model(const std::string& file_path, const material& mat) {
        mesh_3d_model model;

        std::ifstream fileData(file_path);
        if (!fileData) {
            throw std::runtime_error("Could not open file");
        }

        std::string line;
        std::vector<double> vertexData;
        std::vector<double> uData, vData;

        while (std::getline(fileData, line)) {
            std::istringstream stream(line);
            std::string type;
            stream >> type;

            if (type == "v") { // Vertex
                double x, y, z;
                stream >> x >> y >> z;
                vertexData.push_back(x);
                vertexData.push_back(y);
                vertexData.push_back(z);
            }
            else if (type == "vt") { // Texture Coordinate
                double u, v;
                stream >> u >> v;
                uData.push_back(u);
                vData.push_back(v);
            }
            else if (type == "f") { // Face
                std::string vertexInfo[3];
                stream >> vertexInfo[0] >> vertexInfo[1] >> vertexInfo[2];

                std::array<vec3, 3> vertex;
                std::array<vec2, 3> uv_data;
                int index[3];

                for (int i = 0; i < 3; ++i) {
                    std::istringstream vertexStream(vertexInfo[i]);
                    std::string part;
                    getline(vertexStream, part, '/');
                    index[i] = std::stoi(part) - 1;
                    vertex[i] = vec3(vertexData[index[i] * 3],
                                      vertexData[index[i] * 3 + 1],
                                      vertexData[index[i] * 3 + 2]);

                    if (getline(vertexStream, part, '/')) { // Read UV index
                        int uvIndex = std::stoi(part) - 1;
                        uv_data[i] = vec2(uData[uvIndex], vData[uvIndex]);
                    }
                }

                auto tri = make_shared<triangle>(vertex, uv_data, mat);
                model.add(tri);
            }
        }

        return model;
    }
};

#endif //RAYTRACER_OBJ_H
