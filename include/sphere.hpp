#pragma once

#include <common.h>
#include <ray.hpp>
#include <hitable.hpp>
#include <material.hpp>
#include <utils.hpp>

namespace tracer {

class sphere : public hitable
{
public:
    sphere();
    sphere(const glm::vec3& _center, float _radius, material* _mat_ptr);
    virtual bool hit(const ray& r, float t_min, 
                     float t_max, hit_record& record) const override;

    ~sphere();

    glm::vec3 center;
    float radius;
    material* mat_ptr;
};

}