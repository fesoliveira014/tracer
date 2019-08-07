#pragma once

#include <common.h>
#include <glm/glm.hpp>

namespace tracer 
{

class perlin
{
public:
    perlin();
    float noise(const glm::vec3& p) const;
    float turbulence(const glm::vec3& p, int depth) const;

private:
    static std::vector<glm::vec3> generate();
    static void permutate(std::vector<int>& v);
    static std::vector<int> generate_permutation();
    float trilinear_interpolate(glm::vec3 c[2][2][2], float u, float v, float w) const;

private:
    std::vector<glm::vec3> randFloat;
    std::vector<int> permX;
    std::vector<int> permY;
    std::vector<int> permZ;
};

}