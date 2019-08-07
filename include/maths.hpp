#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

namespace tracer {

inline bool refract (const glm::vec3& v, const glm::vec3& n, float index, glm::vec3& refracted)
{
    glm::vec3 uv = glm::normalize(v);
    float dt = glm::dot(uv, n);
    float discriminant = 1.0f - index * index * (1.0f - dt * dt);
    if (discriminant > 0.0f) {
        refracted = index * (uv - dt * n) - std::sqrt(discriminant) * n;
        return true;
    }

    return false;
}

}
