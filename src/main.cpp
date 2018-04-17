#include <tracer.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

glm::vec3 random_in_unit_sphere()
{
    glm::vec3 p;
    do {
        p = 2.0f * glm::vec3(tracer::get_uniform_random(), 
                      tracer::get_uniform_random(), 
                      tracer::get_uniform_random()) - glm::vec3(1.0f);
    } while (glm::length2(p) >= 1.0f);

    return p;
}

glm::vec3 color(const tracer::ray& ray, tracer::hitable& world)
{
    tracer::hit_record rec;
    if (world.hit(ray, 0.001f, std::numeric_limits<float>::max(), rec)) {
        glm::vec3 target = rec.point + rec.normal + random_in_unit_sphere();
        return 0.5f * color(tracer::ray(rec.point, target - rec.point), world);
    }
    else {
        glm::vec3 unit_direction = glm::normalize(ray.direction);
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(.5f, .7f, 1.0f);
    }
}

int main(int argc, char* argv[])
{
    std::string filename("res/output.png");
    if (argc > 1) {
        filename = std::string(argv[1]);
    }

    tracer::image image(500, 250, 3);
    int samples = 50;

    tracer::camera camera{};

    tracer::hitable_list world{};
    world.list.push_back(new tracer::sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f));
    world.list.push_back(new tracer::sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f));

    for (int j = image.get_height() - 1; j >= 0; --j) {
        for (int i = 0; i < image.get_width(); ++i) {
            glm::vec3 col;
            tracer::ray ray;
            for (int n = 0; n < samples; ++n) {
                float u = (float(i) + tracer::get_uniform_random()) / float(image.get_width());
                float v = (float(j) + tracer::get_uniform_random()) / float(image.get_height());
                ray = camera.get_ray(u, v);
                col += color(ray, world);
            }

            col /= float(samples);
            col = glm::sqrt(col);

            int r = int(col.r * 255.99);
            int g = int(col.g * 255.99);
            int b = int(col.b * 255.99);

            image.set_pixel(i, j, glm::ivec3(r,g,b));
        }
    }

    image.save(filename, "png");

    return 0;
}