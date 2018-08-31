#include <sphere.hpp>

namespace tracer {

sphere::sphere() 
    : center{glm::vec3(0.0f)}, radius{0.0f} 
{

}

sphere::sphere(const glm::vec3& _center, float _radius, material* _mat_ptr)
    : center{_center}, radius{_radius}, mat_ptr{_mat_ptr}
{

}

sphere::~sphere()
{
    delete mat_ptr;
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
            record.material_ptr = mat_ptr;
            return true;
        }
        t = (-b + sqrtf(d)) / (2.0f * a);
        if (t < t_max && t > t_min) {
            record.parameter = t;
            record.point = r.point_at_parameter(t);
            record.normal = (record.point - center) / radius;
            record.material_ptr = mat_ptr;
            return true;
        }
    }
    return false;
}

}