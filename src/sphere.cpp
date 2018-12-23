#include <sphere.hpp>

namespace tracer {

sphere::sphere() 
    : center{glm::vec3(0.0f)}, radius{0.0f}, mat_ptr{nullptr},
      speed{glm::vec3(0.0f)}, time0{0.0f}, time1{0.0f} 
{

}

sphere::sphere(const glm::vec3& _center, float _radius, material* _mat_ptr, 
               const glm::vec3& _speed, float t0, float t1)
    : center{_center}, radius{_radius}, mat_ptr{_mat_ptr},
      speed{_speed}, time0{t0}, time1{t1}
{

}

sphere::~sphere()
{
    delete mat_ptr;
}

glm::vec3 sphere::position(float _time) const
{
    glm::vec3 pos = glm::vec3(0.0f);
    if (speed == glm::vec3(0.0f) || time0 == time1) {
        pos = center;
    }
    else {
        glm::vec3 center_t1 = center + speed * (time1 - time0);
        pos = center + ((_time - time0) / (time1 - time0)) * (center_t1 - center);
    }

    return pos;
}

bool sphere::hit(const ray &r, float t_min, 
                 float t_max, hit_record &record) const
{
    glm::vec3 oc = r.origin - position(r.time);
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