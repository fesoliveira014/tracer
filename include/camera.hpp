#pragma once

#include <common.h>
#include <glm/glm.hpp>
#include <ray.hpp>

namespace tracer {
class camera
{
public:
    camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float fov, float aspect) :
        m_position{position},
        m_front{front},
        m_up{up},
        m_fov{fov},
        m_aspect{aspect}
    {
        float theta = m_fov * glm::pi<float>() / 180.0f;
        float halfHeight = glm::tan(theta / 2.f);
        float halfWidth = m_aspect * halfHeight;

        glm::vec3 w = glm::normalize(m_position - m_front);
        glm::vec3 u = glm::normalize(glm::cross(m_up, w));
        glm::vec3 v = glm::cross(w, u);
        
        //m_llc = glm::vec3(-halfWidth, -halfHeight, -1.0f);
        m_llc = m_position - halfWidth * u - halfHeight * v - w;
        m_horizontal = 2 * halfWidth * u;
        m_vertical = 2 * halfHeight * v;
    }

    ray get_ray(float u, float v)
    {
        return ray(m_position, m_llc + u*m_horizontal + v*m_vertical - m_position);
    }

private:
    glm::vec3 m_llc;
    glm::vec3 m_horizontal;
    glm::vec3 m_vertical;

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;

    float m_fov, m_aspect;
    
};
}