#include <common.h>
#include <glm/glm.hpp>
#include <ray.hpp>

namespace tracer {

struct aabb
{
    glm::vec3 min, max;

    aabb() : min{glm::vec3(0)}, max{glm::vec3(0)} {}
    aabb(const glm::vec3& _min, const glm::vec3& _max) : min{_min}, max{_max} {}

    bool hit(const ray& r, float tmin, float tmax) const
    {
        for (int a = 0; a < 3; ++a) {
            float invD = 1.0f / r.direction[a];
            float t0 = (min[a] - r.origin[a]) * invD;
            float t1 = (max[a] - r.origin[a]) * invD;

            if (invD < 0.0f) 
                std::swap(t0, t1);

            tmin = t0 > tmin ? t0 : tmin;
            tmax = t1 < tmax ? t1 : tmax;
            if (tmax <= tmin)
                return false;
        }

        return true;
    }

    static aabb surrounding_box(const aabb& b0, const aabb& b1)
    {
        glm::vec3 small{std::min(b0.min.x, b1.min.x), std::min(b0.min.y, b1.min.y), std::min(b0.min.z, b1.min.z)};
        glm::vec3 big{std::max(b0.max.x, b1.max.x), std::max(b0.max.y, b1.max.y), std::max(b0.max.z, b1.max.z)};

        return aabb(small, big);
    }
};

}