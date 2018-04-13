#include <sphere.hpp>

namespace tracer {

sphere::sphere() 
    : center{glm::vec3(0.0f)}, radius{0.0f} 
{

}

sphere::sphere(glm::vec3 _center, float _radius)
    : center{_center}, radius{_radius}
{

}

bool sphere::hit(const ray &r, float t_min, 
                 float t_max, hit_record &record) const
{
    glm::vec3 oc = r.origin - center;
    float a = glm::dot(r.direction, r.direction);
    float b = 2.0f * glm::dot(oc, r.direction);
    float c = glm::dot(oc, oc) - radius * radius;
    float d = b * b - 4 * a * c;
    if (d > 0.0f) {
        float t = (-b - sqrtf(d)) / (2.0f * a);
        if (t < t_max && t > t_min) {
            record.parameter = t;
            record.point = r.point_at_parameter(t);
            record.normal = (record.point - center) / radius;
            return true;
        }
        t = (-b + sqrtf(d)) / (2.0f * a);
        if (t < t_max && t > t_min) {
            record.parameter = t;
            record.point = r.point_at_parameter(t);
            record.normal = (record.point - center) / radius;
            return true;
        }
    }
    return false;
}

}