#include <iostream>
#include "header.h"
#include "color.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "hittable_list.h"

color ray_color(const ray &r, const hittable &object, int depth);
hittable_list random_scene();
int main()
{
    // Image
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 1200;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 500;
    const int max_depth = 50;

    // Camera 创建一个虚拟视口用于产生光线
    pos lookfrom(13,2,3);
    pos lookat(0,0,0);
    Vector3d vup(0,1,0);
    double dist_to_focus = 10.0;
    double aperture = 0.1;
    camera cam(lookfrom,lookat,vup,20.0, aspect_ratio, aperture, dist_to_focus);

    // World
    // double R = std::cos(pi/4);
    // hittable_list objects;

    // auto material_ground = std::make_shared<lambertian>(color(0.8, 0.8, 0.0));
    // auto material_center = std::make_shared<lambertian>(color(0.1, 0.2, 0.5));
    // auto material_left   = std::make_shared<dielectric>(1.5);
    // auto material_right  = std::make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);
    // // auto material_left   = std::make_shared<lambertian>(color(0,0,1));
    // // auto material_right  = std::make_shared<lambertian>(color(1,0,0));


    // objects.add(std::make_shared<sphere>(pos( 0.0, -100.5, -1.0), 100.0, material_ground));
    // objects.add(std::make_shared<sphere>(pos( 0.0,    0.0, -1.0),   0.5, material_center));
    // objects.add(std::make_shared<sphere>(pos(-1.0,    0.0, -1.0),   0.5, material_left));
    // //将球的半径设为负值, 法相全都翻转到内部。做出一个空心玻璃球:【把一个小球套在大球里, 光线发生两次折射, 负负得正, 上下不会颠倒】
    // objects.add(std::make_shared<sphere>(pos(-1.0,    0.0, -1.0),   -0.45, material_left));
    // objects.add(std::make_shared<sphere>(pos( 1.0,    0.0, -1.0),   0.5, material_right));
    // // objects.add(std::make_shared<sphere>(pos(-R,    0.0, -1.0),   R, material_left));
    // // objects.add(std::make_shared<sphere>(pos( R,    0.0, -1.0),   R, material_right));

    hittable_list world = random_scene();

    // Render
    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
    for (int j = image_height - 1; j >= 0; j--)
        for (int i = 0; i < image_width; i++)
        {
            color pixel_color(0, 0, 0);
            for (int k = 0; k < samples_per_pixel; k++)//同一点的颜色采样多次
            {
                // u,v 为图片对应视口上的坐标
                double u = (i + random_double()) / (image_width - 1);
                double v = (j + random_double()) / (image_height - 1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);
        }
}

color ray_color(const ray &r, const hittable &object, int depth)
{
    hit_record rec;

    if(depth <= 0){//限制反射次数
        return color(0, 0, 0);
    }

    if (object.hit(r, 0.001, infinity, rec))
    {
        ray scattered;
        color attenuation;
        if(rec.mat_ptr->scatter(r, rec, attenuation, scattered)){
            return attenuation*ray_color(scattered, object, depth -1);
        }
        return color(0,0,0);
    }
    // 获取背景色（一个简单的渐变色）
    Vector3d dir = normalize(r.direction());
    double t = (1 + dir.y()) * 0.5;
    return (1 - t) * color(1, 1, 1) + t * color(0.5, 0.7, 1.0);
}

hittable_list random_scene(){
    hittable_list world;
    // 地面大球
    auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
    world.add(std::make_shared<sphere>(pos(0, -1000, 0), 1000, ground_material));
    // 范围内随机生成小球
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            pos center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - pos(4, 0.2, 0)).length() > 0.9) {
                std::shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = std::make_shared<lambertian>(albedo);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = std::make_shared<metal>(albedo, fuzz);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = std::make_shared<dielectric>(1.5);
                    world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    // 生成大球
    auto material1 = std::make_shared<dielectric>(1.5);
    world.add(std::make_shared<sphere>(pos(0, 1, 0), 1.0, material1));

    auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
    world.add(std::make_shared<sphere>(pos(-4, 1, 0), 1.0, material2));

    auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(std::make_shared<sphere>(pos(4, 1, 0), 1.0, material3));

    return world;
}