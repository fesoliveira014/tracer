#pragma once

#include <common.h>
#include <glm/glm.hpp>

namespace tracer {

struct ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    ray() : origin{0.0f}, direction{0.0f} {};
    ray(glm::vec3 o, glm::vec3 d) : origin{o}, direction{d} {};

    glm::vec3 point_at_parameter(float t) const { return origin + direction * t; }
};

}