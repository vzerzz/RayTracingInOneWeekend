#pragma once

#include "vec.h"

class ray
{
public:
    ray() {}
    ray(const pos &origin, const Vector3d &direction)
        : orig(origin), dir(direction) {}

    pos origin() const { return orig; }
    Vector3d direction() const { return dir; }

    pos at(double t) const { return orig + t * dir; }

private:
    pos orig;
    Vector3d dir;
};