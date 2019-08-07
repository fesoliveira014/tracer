#pragma once

#include <common.h>
#include <glm/glm.hpp>
#include <ray.hpp>
#include <utils.hpp>

namespace tracer {
class camera
{
public:
    camera(const glm::vec3& position, const glm::vec3& front, const glm::vec3& up, float fov, float aspect, float aperture, float focusDistance, float t0, float t1) :
        m_position{position},
        m_front{front},
        m_up{up},
        m_fov{fov},
        m_aspect{aspect},
        m_lensRadius{aperture / 2.f},
        m_time0{t0},
        m_time1{t1}
    {
        float theta = m_fov * glm::pi<float>() / 180.0f;
        float halfHeight = glm::tan(theta / 2.f);
        float halfWidth = m_aspect * halfHeight;

        w = glm::normalize(m_position - m_front);
        u = glm::normalize(glm::cross(m_up, w));
        v = glm::cross(w, u);
        
        //m_llc = glm::vec3(-halfWidth, -halfHeight, -1.0f);
        m_llc = m_position - halfWidth * focusDistance * u - halfHeight * focusDistance * v - focusDistance * w;
        m_horizontal = 2 * halfWidth * focusDistance * u;
        m_vertical = 2 * halfHeight * focusDistance * v;
    }

    ray get_ray(float s, float t)
    {
        glm::vec3 rp = m_lensRadius * random_in_unit_disk();
        glm::vec3 offset = u * rp.x + v * rp.y;
        float time = m_time0 + get_uniform_random() * (m_time1 - m_time0);
        return ray(m_position + offset, m_llc + s*m_horizontal + t*m_vertical - m_position - offset, time);
    }

private:
    glm::vec3 m_llc;
    glm::vec3 m_horizontal;
    glm::vec3 m_vertical;

    glm::vec3 m_position;
    glm::vec3 m_front;
    glm::vec3 m_up;

    glm::vec3 u, v, w;

    float m_fov, m_aspect;
    float m_lensRadius;
    float m_time0, m_time1;    
};
}