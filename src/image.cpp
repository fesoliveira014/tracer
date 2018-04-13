#include <image.hpp>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

namespace tracer {

image::image() 
    : _width{0}, _height{0}, _channels{3}, _raw_data{nullptr}
{

}

image::image(uint w, uint h, uint c, byte* data) 
    : _width{w}, _height{h}, _channels{c}, _raw_data{data}
{
    if (_raw_data == nullptr) {
        _raw_data = new byte[_width * _width * _channels];
    }
}

image::image(const image& img)
    : _width{img._width}, _height{img._height}, _raw_data{img._raw_data}
{

}

image::~image()
{
    delete[] _raw_data;
}

bool image::load(std::string filename, image &img)
{
    int w, h, c;
    byte* data = stbi_load(filename.c_str(), &w, &h, &c, 0);

    std::cout << "w, h, c: " << w << "," << h << "," << c << std::endl;

    if (data == nullptr)
        return false;

    img = image(w ,h, c, data);

    return true;
}

bool image::save(std::string filename, std::string extension)
{
    int res;

    stbi_flip_vertically_on_write(1);

    if (extension == "jpg") {
        res = stbi_write_jpg(filename.c_str(), _width, _height, _channels, _raw_data, 50);
        std::cout << "res = " << res << std::endl;
    }

    else if (extension == "tga") {
        res = stbi_write_tga(filename.c_str(), _width, _height, _channels, _raw_data);
    }

    else if (extension == "png") {
        res = stbi_write_png(filename.c_str(), _width, _height, _channels, _raw_data, 0);
    }

    else {
        res = 0;
    }

    return res ? true : false;
}

// bool image::set_pixel(uint x, uint y, glm::ivec3 color, int alpha)
// {
//     if (_raw_data == nullptr || x < 0 || y < 0 || x >= _width || y >= _height)
//         return false;

//     if (_channels == 3) {
//         byte c[3] = {color.r, color.g, color.b};
//         memcpy(_raw_data + (x + y * _width) * _channels, c, _channels);
//     }
//     else if (_channels == 4) {
//         byte c[4] = {color.r, color.g, color.b, alpha};
//         memcpy(_raw_data + (x + y * _width) * _channels, c, _channels);
//     }
// }

bool image::set_pixel(uint x, uint y, const glm::ivec3& color, int alpha)
{
    if (_raw_data == nullptr || x < 0 || y < 0 || x >= _width || y >= _height)
        return false;

    if (_channels == 3)
    {
        byte c[3] = {(byte)(color.r), (byte)(color.g), (byte)(color.b)};
        memcpy(_raw_data + (x + y * _width) * _channels, c, _channels);
    }
    else if (_channels == 4)
    {
        byte c[4] = {(byte)(color.r), (byte)(color.g), (byte)(color.b), (byte)(alpha)};
        memcpy(_raw_data + (x + y * _width) * _channels, c, _channels);
    }

    return true;
}

glm::ivec4 image::get_pixel(uint x, uint y)
{   
    int r, g, b, a;

    r = _raw_data[x + y * _width];
    g = _raw_data[x + y * _width + 1];
    b = _raw_data[x + y * _width + 2];
    a = _channels > 3 ? _raw_data[x + y * _width + 3] : 0;

    return glm::ivec4(r,g,b,a);
}

}