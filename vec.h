#pragma once

#include <cmath>
#include <iostream>

class Vector3d
{
public:
    Vector3d() : ver{0, 0, 0} {}
    Vector3d(double x, double y, double z) : ver{x, y, z} {}

    double x() const { return ver[0]; }
    double y() const { return ver[1]; }
    double z() const { return ver[2]; }

    Vector3d operator-() const { return Vector3d(-ver[0], -ver[1], -ver[2]); }
    double operator[](int i) const { return ver[i]; }
    double &operator[](int i) { return ver[i]; }

    Vector3d &operator+=(const Vector3d &v)
    {
        ver[0] += v.ver[0];
        ver[1] += v.ver[1];
        ver[2] += v.ver[2];
        return *this;
    }

    Vector3d &operator*=(const double t)
    {
        ver[0] *= t;
        ver[1] *= t;
        ver[2] *= t;
        return *this;
    }

    Vector3d &operator/=(const double t)
    {
        return *this *= 1 / t;
    }

    double norm() const { return ver[0] * ver[0] + ver[1] * ver[1] + ver[2] * ver[2]; }
    double length() const { return std::sqrt(norm()); }

    inline static Vector3d random(){
        return Vector3d(random_double(), random_double(), random_double());
    }

    inline static Vector3d random(double min, double max){
        return Vector3d(random_double(min,max), random_double(min,max), random_double(min,max));
    }

    bool near_zero() const{
        // Return true if the vector is close to zero in all dimensions.
        const auto s = 1e-8;
        return (fabs(ver[0] < s) && fabs(ver[1] < s) && fabs(ver[2] < s));
    }
public:
    double ver[3];
};
// aliases
using pos = Vector3d;
using color = Vector3d;
// Vector3 Utility Functions
inline std::ostream &operator<<(std::ostream &out, const Vector3d &v)
{
    return out << v.ver[0] << ' ' << v.ver[1] << ' ' << v.ver[2];
}
inline Vector3d operator+(const Vector3d &u, const Vector3d &v)
{
    return Vector3d(u.ver[0] + v.ver[0], u.ver[1] + v.ver[1], u.ver[2] + v.ver[2]);
}
inline Vector3d operator-(const Vector3d &u, const Vector3d &v)
{
    return Vector3d(u.ver[0] - v.ver[0], u.ver[1] - v.ver[1], u.ver[2] - v.ver[2]);
}
inline Vector3d operator*(const Vector3d &u, const Vector3d &v)
{
    return Vector3d(u.ver[0] * v.ver[0], u.ver[1] * v.ver[1], u.ver[2] * v.ver[2]);
}
inline Vector3d operator*(double t, const Vector3d &v)
{
    return Vector3d(t * v.ver[0], t * v.ver[1], t * v.ver[2]);
}
inline Vector3d operator*(const Vector3d &v, double t)
{
    return t * v;
}
inline Vector3d operator/(const Vector3d &v, double t)
{
    return (1 / t) * v;
}
inline double dot(const Vector3d &u, const Vector3d &v)
{
    return u.ver[0] * v.ver[0] + u.ver[1] * v.ver[1] + u.ver[2] * v.ver[2];
}
inline Vector3d cross(const Vector3d &u, const Vector3d &v)
{
    return Vector3d(u.ver[1] * v.ver[2] - u.ver[2] * v.ver[1],
                    u.ver[2] * v.ver[0] - u.ver[0] * v.ver[2],
                    u.ver[0] * v.ver[1] - u.ver[1] * v.ver[0]);
}
inline Vector3d normalize(const Vector3d &v)
{
    return v / v.length();
}

Vector3d random_unit_sphere(){//rejection method 生成单位球内的随机点
    while (true)
    {
        Vector3d p = Vector3d::random(-1,1);
        if(p.length() >= 1) continue;
        return p;
    }
}

Vector3d random_unit_vector(){
    double a = random_double(0, 2*pi);
    double z = random_double(-1, 1);
    double r = sqrt(1-z*z);
    return Vector3d(r*std::cos(a), r*std::sin(a), z);
}

Vector3d random_in_hemisphere(const Vector3d& normal) {
    Vector3d in_unit_sphere = random_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vector3d reflect(const Vector3d& v, const Vector3d& n){
    return v - 2*dot(v, n)*n;
}

// uv 为入射向量 n 为表面法线向量，均为单位向量 etai_over_etat为介质的折射率之比
Vector3d refract(const Vector3d& uv, const Vector3d& n, double etai_over_etat){
    double cos_theta = fmin(dot(-uv, n), 1.0);
    Vector3d r_out_perp = etai_over_etat*(uv + cos_theta*n);
    Vector3d r_out_parallel = -std::sqrt(fabs(1.0 - r_out_perp.norm()))*n;
    return r_out_perp + r_out_parallel;
}

Vector3d random_in_unit_disk(){
    while(true){
        Vector3d p = Vector3d(random_double(-1, 1), random_double(-1, 1), 0);
        if(p.norm() >= 1) continue;
        return p;
    }
}