#pragma once

#include <common.h>
#include <glm/glm.hpp>

namespace tracer {
class image
{
protected:
    uint _width, _height; // in pixels
    uint _channels;
    byte* _raw_data;

public:
    image();
    image(uint w, uint h, uint c, byte* data = nullptr);
    image(image const& img);

    ~image();

    uint get_width() { return _width; }
    uint get_height() { return _height; }
    byte* get_raw_data() { return _raw_data; }

    static bool load(std::string filename, image& img);
    bool save(std::string filename, std::string extension);

    //bool set_pixel(uint x, uint y, glm::ivec3 color, int alpha = 0);
    bool set_pixel(uint x, uint y, const glm::ivec3 &color, int alpha = 0);

    glm::ivec4 get_pixel(uint x, uint y);
};
}