#pragma once

#include <common.h>
#include <ray.hpp>
#include <hitable.hpp>
#include <utils.hpp>
#include <maths.hpp>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

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
        scattered = ray(record.point, target - record.point, incoming.time);
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
        scattered = ray(record.point, reflected + fuzz * random_in_unit_sphere(), incoming.time);
        attenuation = albedo;
        return glm::dot(scattered.direction, record.normal) > 0.0;
    }
};

struct dieletric : public material
{
    float refractionIndex;
    mutable std::default_random_engine generator;
    mutable std::uniform_real_distribution<float> distribution;

    dieletric(float ri) : refractionIndex{ri}, generator{}, distribution{0.0f, 1.0f} {}

    float schlick(float cosine, float refractionIndex) const
    {
        float r0 = (1.0f - refractionIndex) / (1.0f + refractionIndex);
        r0 *= r0;
        return r0 + (1.0f - r0) * std::pow((1.0f - cosine), 5.0f);
    }

    bool scatter(const ray& incoming, const hit_record& record, glm::vec3& attenuation, ray& scattered) const
    {
        // snells law:
        // n * sin(theta) = n' * sin(theta')

        glm::vec3 outwardNormal;
        glm::vec3 reflected = glm::reflect(glm::normalize(incoming.direction), record.normal);
        float index;
        attenuation = glm::vec3(1.f, 1.f, 1.f);
        glm::vec3 refracted{0.0f};
        float reflectionProbability;
        float cosine;

        if (glm::dot(incoming.direction, record.normal) > 0.f) {
            outwardNormal = -record.normal;
            index = refractionIndex;
            cosine = refractionIndex * glm::dot(incoming.direction, record.normal) / glm::length(incoming.direction);
        }
        else {
            outwardNormal = record.normal;
            index = 1.f / refractionIndex;
            cosine = -glm::dot(incoming.direction, record.normal) / glm::length(incoming.direction);
        }

        if (refract(incoming.direction, outwardNormal, index, refracted)) {
            reflectionProbability = schlick(cosine, refractionIndex);
        }
        else {
            reflectionProbability = 1.0f;
        }

        if (distribution(generator) < reflectionProbability) {
            scattered = ray(record.point, reflected, incoming.time);
        }
        else {
            scattered = ray(record.point, refracted, incoming.time);
        }

        return true;
    }

};

}