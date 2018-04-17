#pragma once

#include <common.h>

namespace tracer {

inline float get_uniform_random()
{
    static std::random_device rd;
    static std::uniform_real_distribution<float> dist(0.0f, 1.0f);

    return dist(rd);
}

}