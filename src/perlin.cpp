#include <perlin.hpp>
#include <utils.hpp>

namespace tracer
{

perlin::perlin() :
    randFloat{generate()},
    permX{generate_permutation()},
    permY{generate_permutation()},
    permZ{generate_permutation()}
{
}

float perlin::noise(const glm::vec3& p) const
{
    float u = p.x - std::floor(p.x);
    float v = p.y - std::floor(p.y);
    float w = p.z - std::floor(p.z);

    int i = std::floor(p.x);
    int j = std::floor(p.y);
    int k = std::floor(p.z);

    glm::vec3 c[2][2][2];

    for (int x = 0; x < 2; ++x) {
        for (int y = 0; y < 2; ++y) {
            for (int z = 0; z < 2; ++z) {
                c[x][y][z] = randFloat[permX[(i + x) & 255] ^ permY[(j + y) & 255] ^ permZ[(k + z) & 255]];
            }
        }
    }

    return trilinear_interpolate(c, u, v, w);
}

std::vector<glm::vec3> perlin::generate()
{
    std::vector<glm::vec3> p(256);
    for (size_t i = 0; i < 256; ++i) {
        p[i] = glm::normalize(glm::vec3{-1 + 2 * get_uniform_random(),
                                        -1 + 2 * get_uniform_random(),
                                        -1 + 2 * get_uniform_random()});
    }
    return p;
}

void perlin::permutate(std::vector<int>& v)
{
    for (size_t i = v.size() - 1; i < v.size(); --i) {
        int target = int(get_uniform_random() * (i + 1));
        std::swap(v[i], v[target]);
    	}
}

std::vector<int> perlin::generate_permutation()
{
    std::vector<int> p(256);
    for (int i = 0; i < p.size(); ++i) {
        p[i] = i;
    }
    permutate(p);
    return p;
}

float perlin::trilinear_interpolate(glm::vec3 c[2][2][2], float u, float v, float w) const
{
    float uu = u * u * (3 - 2*u);
    float vv = v * v * (3 - 2*v);
    float ww = w * w * (3 - 2*w);

    float accum = 0;
    for (int i = 0; i < 2; ++i) {
        for (int j = 0; j < 2; ++j) {
            for (int k = 0; k < 2; ++k) {
                glm::vec3 weigth{u-i, v-j, w-k};
                accum += (i*uu + (1-i) * (1-uu)) *
                         (j*vv + (1-j) * (1-vv)) *
                         (k*ww + (1-k) * (1-ww)) * 
                         glm::dot(c[i][j][k], weigth);
            }
        }
    }
    return accum;
}

float perlin::turbulence(const glm::vec3& p, int depth) const
{
    float accum = 0;
    glm::vec3 tmp{p};
    float weigth = 1.0f;

    for (int i = 0; i < depth; ++i) {
        accum += weigth * noise(tmp);
        weigth *= 0.5f;
        tmp *= 2;
    }

    return std::fabs(accum);
}

}