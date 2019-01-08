#pragma once

#include <common.h>
#include <ray.hpp>
#include <aabb.hpp>

#include <glm/glm.hpp>

namespace tracer {

struct material;

struct hit_record
{
    float parameter;
    glm::vec3 point;
    glm::vec3 normal;
    material* material_ptr;

    hit_record() : parameter{0.0f}, point{}, normal{}, material_ptr{nullptr} {}
    hit_record(const hit_record& h) : 
        parameter{h.parameter}, point{h.point}, normal{h.normal}, material_ptr{h.material_ptr} {}
};

class hitable
{
public:
    virtual ~hitable() {};
    virtual bool hit(const ray& r, float t_min, 
        float t_max, hit_record& record) const = 0;
    virtual bool bounding_box(float t0, float t1, aabb& box) const = 0;

};

class hitable_list : public hitable
{

public:
    hitable_list();
    hitable_list(const std::vector<hitable*>& _list);
    hitable_list(const hitable_list& hl);
    hitable_list(hitable_list&& hl);
    
    ~hitable_list();
    virtual bool hit(const ray &r, float t_min,
                     float t_max, hit_record &record) const override;
    virtual bool bounding_box(float t0, float t1, aabb& box) const override;
    
    std::vector<hitable*> list;
};

}