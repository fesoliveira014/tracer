#pragma once

#include <common.h>
#include <glm/glm.hpp>
#include <ray.hpp>

namespace tracer {
class camera
{
public:
    camera()
    {
        llc = glm::vec3(-2.0f, -1.0f, -1.0f);
        horizontal = glm::vec3(4.0f, 0.0f, 0.0f);
        vertical = glm::vec3(0.0f, 2.0f, 0.0f);
        origin = glm::vec3(0.0f);
    }

    ray get_ray(float u, float v)
    {
        return ray(origin, llc + u*horizontal + v*vertical - origin);
    }

    glm::vec3 llc;
    glm::vec3 horizontal;
    glm::vec3 vertical;
    glm::vec3 origin;
    
};
}