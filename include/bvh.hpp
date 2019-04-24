#pragma once

#include <common.h>
#include <hitable.hpp>

namespace tracer
{

class bvh_node : public hitable
{
public:
    bvh_node();
    bvh_node(std::vector<hitable_ptr> list, float t0, float t1);
    ~bvh_node();

    virtual bool hit(const ray& r, float t_min, float t_max, 
                hit_record& record) const override;
    virtual bool bounding_box(float t0, float t1, aabb& box) const override;

    hitable_ptr left;
    hitable_ptr right;

    aabb bbox;

    bool is_leaf;

protected:
};

}