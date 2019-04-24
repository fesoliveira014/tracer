#pragma once

#include <common.h>

#include <glm/glm.hpp>

namespace tracer
{

struct texture 
{
    virtual glm::vec3 value(float u, float v, const glm::vec3& p) const = 0;
};

typedef std::shared_ptr<texture> texture_ptr;

struct constant_texture : public texture
{
    glm::vec3 color;

    constant_texture() {}
    constant_texture(const glm::vec3& c) : color{c} {}

    glm::vec3 value(float u, float v, const glm::vec3& p) const override
    {
        return color;
    }
};

struct checker_texture : public texture
{
    texture_ptr even;
    texture_ptr odd;

    checker_texture() {}
    checker_texture(texture_ptr t0, texture_ptr t1) : even(t0), odd(t1) {}

    glm::vec3 value(float u, float v, const glm::vec3& p) const override
    {
        float sines = glm::sin(10 * p.x) * glm::sin(10 * p.y) * glm::sin(10 * p.z);
        if (sines < 0) 
            return odd->value(u, v, p);
        return even->value(u, v, p);
    }
    
};

}