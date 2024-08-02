//
// Created by Hanson Siu on 30/07/2024.
//

#ifndef RAYTRACER_VEC2_H
#define RAYTRACER_VEC2_H

#include <cmath>
#include <cstdlib>

class vec2 {
public:
    double e[2];

    vec2() : e{0, 0} {}
    vec2(double e0, double e1) : e{e0, e1} {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }

    vec2 operator-() const { return {-e[0], -e[1]}; }
    double operator[](int i) const { return e[i]; }
    double& operator[](int i) { return e[i]; }

    vec2& operator+=(const vec2& v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        return *this;
    }

    vec2& operator-=(const vec2& v) {
        e[0] -= v.e[0];
        e[1] -= v.e[1];
        return *this;
    }

    vec2& operator*=(double t) {
        e[0] *= t;
        e[1] *= t;
        return *this;
    }

    vec2& operator/=(double t) {
        return *this *= (1.0 / t);
    }

    double dot(const vec2& v) const {
        return e[0] * v.e[0] + e[1] * v.e[1];
    }

    // Cross product for 2D (returns a vec2 representing the perpendicular direction)
    vec2 cross(const vec2& v) const {
        return vec2(e[0] * v.e[1] - v.e[0] * e[1],
                    y() * v.x() - v.y() * x());
    }
};

// Overloaded operators
inline vec2 operator+(const vec2& u, const vec2& v) {
    return {u.e[0] + v.e[0], u.e[1] + v.e[1]};
}

inline vec2 operator-(const vec2& u, const vec2& v) {
    return {u.e[0] - v.e[0], u.e[1] - v.e[1]};
}

inline vec2 operator*(const vec2& v, double t) {
    return {v.e[0] * t, v.e[1] * t};
}

inline vec2 operator*(double t, const vec2& v) {
    return v * t;
}

inline vec2 operator/(const vec2& v, double t) {
    return (1.0 / t) * v;
}

#endif //RAYTRACER_VEC2_H
