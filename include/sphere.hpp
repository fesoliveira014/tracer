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
    sphere(const glm::vec3& _center, float _radius, material_ptr _mat_ptr, 
           const glm::vec3& _speed = glm::vec3(0.0f), float t0 = 0.0f, float t1 = 0.0f);
    virtual bool hit(const ray& r, float t_min, 
                     float t_max, hit_record& record) const override;
    virtual bool bounding_box(float t0, float t1, aabb& box) const override;

    ~sphere();

    glm::vec3 position(float _time) const;

    glm::vec3 center;
    float radius;
    material_ptr mat_ptr;

    glm::vec3 speed;
    float time0, time1;
};

}