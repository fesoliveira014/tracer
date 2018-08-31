#pragma once

#include <common.h>
#include <glm/glm.hpp>

namespace tracer {

inline float get_uniform_random()
{
    static std::random_device rd;
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    return dist(rd);
}

inline glm::vec3 random_in_unit_sphere()
{
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(get_uniform_random(), 
                      get_uniform_random(), 
                      get_uniform_random()) - glm::vec3(1.0f);
    } while (glm::length(p) >= 1.0f);

    return p;
}

}