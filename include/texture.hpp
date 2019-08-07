#pragma once

#include <common.h>

#include <glm/glm.hpp>
#include <perlin.hpp>

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

class noise_texture : public texture
{
public:
    noise_texture() : noise{}, scale{1.0f} {}
    noise_texture(float s) : noise{}, scale{s} {}

    glm::vec3 value(float u, float v, const glm::vec3& p) const override
    {
        return glm::vec3(1,1,1) * (1.0f + noise.noise(scale * p)) * 0.5f ; 
    }  
private:
    perlin noise;
    float scale;
};

class turbulent_noise_texture : public texture
{
public:
    turbulent_noise_texture() : noise{}, scale{1.0f}, depth{7} {}
    turbulent_noise_texture(float s, int d = 7) : noise{}, scale{s}, depth{d} {}

    glm::vec3 value(float u, float v, const glm::vec3& p) const override
    {
        return glm::vec3(1,1,1) * noise.turbulence(scale * p, depth); 
    }  
private:
    perlin noise;
    float scale;
    int depth;
};

class marble_texture : public texture
{
public:
    marble_texture() : noise{}, scale{1.0f}, depth{7} {}
    marble_texture(float s, int d = 7) : noise{}, scale{s}, depth{d} {}

    glm::vec3 value(float u, float v, const glm::vec3& p) const override
    {
        return glm::vec3(1,1,1) * 0.5f * (1.0f  + std::sin(scale * p.z + 10 * noise.turbulence(scale * p, depth))); 
    }  
private:
    perlin noise;
    float scale;
    int depth;
};

}