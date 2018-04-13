#pragma once

#include <common.h>
#include <ray.hpp>
#include <hitable.hpp>

namespace tracer {

class sphere : public hitable
{
public:
    sphere();
    sphere(glm::vec3 _center, float _radius);
    virtual bool hit(const ray& r, float t_min, 
                     float t_max, hit_record& record) const override;

    glm::vec3 center;
    float radius;
};

}