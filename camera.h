#pragma once
#include "header.h"

class camera{
    public:
        camera(
            pos lookfrom,
            pos lookat,
            Vector3d vup,
            double vfov, // vertical field-of-view in degrees
            double aspect_ratio,
            double aperture,// 光圈大小
            double focus_dist// 焦距
        ) {
            double theta = deg2rad(vfov);
            double h = std::tan(theta/2);
            double viewport_height = 2.0 * h;
            double viewport_width = aspect_ratio * viewport_height;
            // wuv为摄像机自己的坐标系
            w = normalize(lookfrom - lookat);
            u = normalize(cross(vup,w));
            v = cross(w, u);

            origin = lookfrom;
            horizontal =focus_dist* viewport_width* u;
            vertical = focus_dist* viewport_height* v;
            lower_left_corner = origin - horizontal/2 - vertical/2 - focus_dist* w;
            lens_radius = aperture/2;
        }

        // u,v 为图片对应视口上的坐标
        ray get_ray(double s, double t) const {
            //光线从lookfrom处的虚拟透镜发出 透镜半径越大越模糊
            Vector3d rd = lens_radius * random_in_unit_disk();
            Vector3d offset = u*rd.x() + v*rd.y();

            return ray(origin + offset, lower_left_corner + s*horizontal + t*vertical - origin - offset);
        }

    private:
        pos origin;
        pos lower_left_corner;
        Vector3d horizontal;
        Vector3d vertical;
        Vector3d u,v,w;
        double lens_radius;
};