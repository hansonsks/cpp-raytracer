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
        material/dielectric.h)
