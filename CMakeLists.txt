cmake_minimum_required(VERSION 3.26)
project(Raytracer)

set(CMAKE_CXX_STANDARD 11)

add_executable(Raytracer main.cpp
        geometry/vec3.h
        geometry/colour.h
        geometry/ray.h
        hittable/hittable.h
        hittable/sphere.h
        hittable/hittable_list.h
        utils/common.h
        utils/interval.h
        camera.h
        material/material.h
        material/lambertian.h
        material/metal.h
        material/dielectric.h
        geometry/aabb.h
        hittable/bvh.h
        texture/constant_medium.h
        material/diffuse_light.h
        material/isotropic.h
        texture/perlin.h
        geometry/quad.h
        rtw_stb_image.h
        external/stb_image.h
        external/stb_image_write.h
        texture/texture.h
        utils/pdf.h
        utils/onb.h
        texture/noise_texture.h
        texture/image_texture.h
        texture/checker_texture.h
        texture/solid_colour.h
        geometry/OBJ.h
        geometry/mesh_3d_model.h
        geometry/vec2.h
        geometry/triangle.h
        geometry/sphere.h
)
