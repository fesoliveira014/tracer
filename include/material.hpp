#pragma once

#include <common.h>
#include <ray.hpp>
#include <hitable.hpp>
#include <utils.hpp>

#include <glm/glm.hpp>

namespace tracer {


struct material
{
    virtual ~material() {}
    virtual bool scatter(const ray& incoming, const hit_record& record, glm::vec3& attenuation, ray& scattered) const = 0;
};

struct lambertian : public material
{
    glm::vec3 albedo;
    
    lambertian(const glm::vec3& a) : albedo{a} {};

    bool scatter(const ray& incoming, const hit_record& record, glm::vec3& attenuation, ray& scattered) const
    {
        glm::vec3 target = record.point + record.normal + random_in_unit_sphere();
        scattered = ray(record.point, target - record.point);
        attenuation = albedo;
        return true;
    }
};

struct metal : public material
{
    glm::vec3 albedo;
    float fuzz;

    metal(const glm::vec3& a, float f) : albedo{a}, fuzz{f} {}

    bool scatter(const ray& incoming, const hit_record& record, glm::vec3& attenuation, ray& scattered) const
    {
        glm::vec3 reflected = glm::reflect(glm::normalize(incoming.direction), record.normal);
        scattered = ray(record.point, reflected + fuzz * random_in_unit_sphere());
        attenuation = albedo;
        return glm::dot(scattered.direction, record.normal) > 0.0;
    }
};

}