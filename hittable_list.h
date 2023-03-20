#pragma once

#include "hittable.h"
#include <memory>
#include <vector>

class hittable_list : public hittable{
    public:
        hittable_list(){}
        hittable_list(std::shared_ptr<hittable> object){
            add(object);
        }
        void clear(){objects.clear();}
        void add(std::shared_ptr<hittable> object){
            objects.push_back(object);
        }
        virtual bool hit(const ray &r, double t_min, double t_max, hit_record& rec) const override;

    public:
        std::vector<std::shared_ptr<hittable>> objects;
};

bool hittable_list::hit(const ray &r, double t_min, double t_max, hit_record& rec) const{
    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;
    // 如果有一个对象与射线相交了，则把相交点的时间t记录为下次相交的最大t，
    // 以便仅仅求解最近的相交
    for(const auto& object : objects){
        if(object -> hit(r, t_min, closest_so_far, temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}
