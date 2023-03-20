#pragma once

#include "header.h"
#include "hittable.h"

//struct hit_record;

class material
{
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian: public material{
    public:
        lambertian(const Vector3d& a) : albedo(a){}
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            Vector3d scatter_direction = rec.normal + random_unit_vector();
            
            //如果我们生成的随机单位向量与法向量正好相反，则两者之和将为零，这将导致零散射方向向量。
            if(scatter_direction.near_zero())
                scatter_direction = rec.normal;
            
            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }


    public:
        Vector3d albedo;// 散射衰减值
};

class metal: public material{
    public:
        metal(const color& a, double f) : albedo(a), fuzz(f) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            Vector3d reflected = reflect(normalize(r_in.direction()), rec.normal);
            scattered = ray(rec.p, reflected + fuzz*random_unit_sphere());
            attenuation = albedo;
            return (dot(scattered.direction(), rec.normal) > 0);
        }
    public:
        color albedo;
        double fuzz;//模拟金属的粗糙度 fuzz越大, 也就是随机球的半径很大, 光线可能会散射到物体内部去。这时候我们可以认为物体吸收了光线
};

class dielectric: public material{
    public:
        dielectric(double index_of_refraction) : ir(index_of_refraction) {}
        virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override{
            double refraction_ratio = rec.front_face ? (1.0/ir): ir;

            Vector3d unit_direction = normalize(r_in.direction());
            double cos_theta = fmin(dot(-unit_direction, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta*cos_theta);
            Vector3d direction;

            if((refraction_ratio * sin_theta > 1.0) || reflectance(cos_theta, refraction_ratio) > random_double())//全反射以及发生折射的概率会随着入射角而改变(近似)
                direction = reflect(unit_direction, rec.normal);
            else
                direction = refract(unit_direction, rec.normal, refraction_ratio);

            scattered = ray(rec.p, direction);
            attenuation = color(1.0,1.0,1.0);//不衰减, 玻璃表面不吸收光的能量
            return true;
        }

    public:
        double ir;//折射率 Index of Refraction

    private:
        static double reflectance(double cosine, double ref_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1-ref_idx) / (1+ref_idx);
            r0 = r0*r0;
            return r0 + (1-r0)*pow((1 - cosine),5);
        }
};