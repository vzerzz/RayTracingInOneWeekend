#pragma once

#include "hittable.h"
#include "vec.h"

class sphere : public hittable
{
public:
    sphere() {}
    sphere(pos c, double r, std::shared_ptr<material> m)
        : center(c), radius(r), mat_ptr(m) {}
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record& rec) const override;

public:
    pos center;
    double radius;
    std::shared_ptr<material> mat_ptr;
};

bool sphere::hit(const ray &r, double t_min, double t_max, hit_record& rec) const
{
    Vector3d c2o = r.origin() - center;
    double a = dot(r.direction(), r.direction());
    double half_b = dot(r.direction(), c2o);
    double c = dot(c2o, c2o) - radius * radius;
    if ((half_b * half_b - a * c) < 0)
        return false;
    double t = (-half_b - std::sqrt(half_b * half_b - a * c)) / a;
    if (t < t_min || t > t_max)
    {
        t = (-half_b + std::sqrt(half_b * half_b - a * c)) / a;
        if (t < t_min || t > t_max)
        {
            return false;
        }
    }
    rec.t = t;
    rec.p = r.at(t);
    Vector3d out_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, out_normal);
    rec.mat_ptr = mat_ptr;
    return true;
}
