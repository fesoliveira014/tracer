#include <tracer.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

glm::vec3 color(const tracer::ray& ray, tracer::hitable& world, int depth)
{
    tracer::hit_record rec;
    if (world.hit(ray, 0.001f, std::numeric_limits<float>::max(), rec)) {
        tracer::ray scattered;
        glm::vec3 attenuation;
        if (depth < 50 && rec.material_ptr->scatter(ray, rec, attenuation, scattered)) 
            return attenuation * color(scattered, world, depth + 1);
        else
            return glm::vec3(0.0f);
    }
    else {
        glm::vec3 unit_direction = glm::normalize(ray.direction);
        float t = 0.5f * (unit_direction.y + 1.0f);
        return (1.0f - t) * glm::vec3(1.0f) + t * glm::vec3(.11f, .55f, 0.95f);
    }
}

int main(int argc, char* argv[])
{
    std::string filename("res/output.png");
    if (argc > 1) {
        filename = std::string(argv[1]);
    }

    tracer::image image(500, 250, 3);
    int nsamples = 50;

    tracer::camera camera{};

    tracer::hitable_list world{};
    world.list.push_back(new tracer::sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f,new tracer::lambertian(glm::vec3(0.8f, 0.3f, 0.3f))));
    world.list.push_back(new tracer::sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new tracer::lambertian(glm::vec3(0.8f, 0.8f, 0.0f))));
    world.list.push_back(new tracer::sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new tracer::lambertian(glm::vec3(0.8f, 0.8f, 0.0f))));
    world.list.push_back(new tracer::sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new tracer::metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.3f)));
    world.list.push_back(new tracer::sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new tracer::dieletric(1.3f)));

    int r, g, b;
    glm::vec3 col;
    tracer::ray ray;

    std::stringstream stream1, stream2;

    for (int j = image.get_height() - 1; j >= 0; --j) {
        for (uint32_t i = 0; i < image.get_width(); ++i) {
            for (int n = 0; n < nsamples; ++n) {
                float u = (float(i) + tracer::get_uniform_random()) / float(image.get_width());
                float v = (float(j) + tracer::get_uniform_random()) / float(image.get_height());
                ray = camera.get_ray(u, v);
                col += color(ray, world, 0);
            }
            

            col /= float(nsamples);
            col = glm::sqrt(glm::abs(col));
            // stream1 << "color = (" << col.r << "," << col.g << "," << col.b << ")" << std::endl;

            r = int(col.r * (float)254.99);
            g = int(col.g * (float)254.99);
            b = int(col.b * (float)254.99);
            // stream2 << "color = (" << r << "," << g << "," << b << ")" << std::endl;

            image.set_pixel(i, j, glm::ivec3(r,g,b));

        }
    }

    image.save(filename, "png");

    // std::cout << stream1.str();
    // std::cout << stream2.str();

    return 0;
}