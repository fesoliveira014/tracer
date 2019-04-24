#pragma once

#include <common.h>
#include <ray.hpp>
#include <aabb.hpp>
// #include <material.hpp>

#include <glm/glm.hpp>

namespace tracer {

struct material;

struct hit_record
{
    using material_ptr = std::shared_ptr<material>;

    float parameter;
    glm::vec3 point;
    glm::vec3 normal;
    material_ptr mat;

    hit_record() : parameter{0.0f}, point{}, normal{}, mat{nullptr} {}
    hit_record(const hit_record& h) : 
        parameter{h.parameter}, point{h.point}, normal{h.normal}, mat{h.mat} {}
};

class hitable
{
public:
    virtual ~hitable() {};
    virtual bool hit(const ray& r, float t_min, 
        float t_max, hit_record& record) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;

};

typedef std::shared_ptr<hitable> hitable_ptr;

class hitable_list : public hitable
{

public:
    hitable_list();
    hitable_list(const std::vector<hitable_ptr>& _list);
    hitable_list(const hitable_list& hl);
    hitable_list(hitable_list&& hl);
    
    ~hitable_list();
    virtual bool hit(const ray &r, float t_min,
                     float t_max, hit_record &record) const override;
    virtual bool bounding_box(float t0, float t1, aabb& box) const override;
    
    std::vector<hitable_ptr> list;
};

}