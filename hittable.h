#pragma once

#include "ray.h"

class material;

struct hit_record
{
    pos p;
    Vector3d normal;
    std::shared_ptr<material> mat_ptr;
    double t;
    bool front_face;
    //使法线始终和入射光线方向相反
    inline void set_face_normal(const ray&r, const Vector3d& out_normal){
        front_face = dot(r.direction(), out_normal) < 0;
        normal = front_face ? out_normal: -out_normal;
    }
};

class hittable
{
public:
    virtual bool hit(const ray &r, double t_min, double t_max, hit_record& rec) const = 0;
};
