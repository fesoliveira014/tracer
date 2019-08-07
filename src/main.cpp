#include <tracer.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>


glm::vec3 color(const tracer::ray& ray, tracer::hitable& world, int depth)
{
    tracer::hit_record rec;
    if (world.hit(ray, 0.001f, std::numeric_limits<float>::max(), rec)) {
        tracer::ray scattered;
        glm::vec3 attenuation;
        if (depth < 50 && rec.mat->scatter(ray, rec, attenuation, scattered)) 
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

tracer::hitable_list generate_scene()
{
    tracer::hitable_list world{};

    tracer::texture_ptr checker{new tracer::checker_texture{tracer::texture_ptr{new tracer::constant_texture(glm::vec3(.2f, .3f, .1f))}, 
                                                            tracer::texture_ptr{new tracer::constant_texture(glm::vec3(.9f, .9f, .9f))}}};
    
    world.list.push_back(tracer::hitable_ptr{new tracer::sphere(
                                                glm::vec3(0.f, -1000.f, 0.f), 
                                                1000.f, 
                                                tracer::material_ptr(
                                                    new tracer::lambertian(checker)))});

    for (int a = -11; a < 11; ++a) {
        for (int b = -11; b < 11; ++b) {
            float chooseMaterial = tracer::get_uniform_random();
            glm::vec3 center(a + 0.9f + tracer::get_uniform_random(), 
                             0.2f, b + .9f * tracer::get_uniform_random());
            if (glm::length(center - glm::vec3{4.f, .2f, 0.f}) > .9f) {
                if (chooseMaterial < 0.8f) {
                    glm::vec3 speed = glm::vec3(0.0f, 0.5f * tracer::get_uniform_random(), 0.0f);
                    world.list.push_back(tracer::hitable_ptr{new tracer::sphere(center, 0.2f, 
                                                             tracer::material_ptr(
                                                                 new tracer::lambertian(
                                                                     tracer::texture_ptr(
                                                                         new tracer::constant_texture(
                                                                            glm::vec3(tracer::get_uniform_random(), 
                                                                                      tracer::get_uniform_random(), 
                                                                                      tracer::get_uniform_random()))))),
                                                             speed, 0.0f, 1.0f)});
                }
                else if (chooseMaterial < 0.95f) {
                    world.list.push_back(tracer::hitable_ptr{new tracer::sphere(center, 0.2f,
                                                                                tracer::material_ptr(new tracer::metal(glm::vec3(0.5f * (1.f + tracer::get_uniform_random()),
                                                                                                            0.5f * (1.f + tracer::get_uniform_random()),
                                                                                                            0.5f * (1.f + tracer::get_uniform_random())),
                                                                                                  0.5f * tracer::get_uniform_random())))});
                }
                else {
                    world.list.push_back(tracer::hitable_ptr{new tracer::sphere(center, 0.2f, tracer::material_ptr(new tracer::dieletric(1.5f)))});
                    if (chooseMaterial < 0.97f)
                        world.list.push_back(tracer::hitable_ptr{new tracer::sphere(center, -0.18f, tracer::material_ptr(new tracer::dieletric(1.5f)))});
                }
            }
        }
    }

    world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(0.f, 1.f, 0.f), 1.f,  tracer::material_ptr(new tracer::dieletric(1.5f)))});
    world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(-4.f, 1.f, 0.f), 1.f, tracer::material_ptr(new tracer::lambertian(tracer::texture_ptr(new tracer::constant_texture(glm::vec3(0.4f, 0.2f, 0.1f))))))});
    world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(4.f, 1.f, 0.f), 1.f,  tracer::material_ptr(new tracer::metal(glm::vec3(0.7f, 0.6f, 0.5f), 0.0f)))});

    return world;
}

tracer::hitable_list two_spheres()
{
    tracer::texture_ptr checker{
        new tracer::checker_texture {
            tracer::texture_ptr{ new tracer::constant_texture(glm::vec3(.2f, .3f, .1f)) },
            tracer::texture_ptr{ new tracer::constant_texture(glm::vec3(.9f, .9f, .9f)) }
        }  
    };

    tracer::texture_ptr perlin{
      new tracer::noise_texture{10.f}
    };

    tracer::texture_ptr turbulent{
      new tracer::turbulent_noise_texture{10.f}
    };

    tracer::texture_ptr marble{
      new tracer::marble_texture{4.f}
    };

    // int n = 50;

    tracer::hitable_list list{};
    list.list.push_back(tracer::hitable_ptr{new tracer::sphere{glm::vec3{.0, -1000., .0}, 1000.0, tracer::material_ptr{new tracer::lambertian{perlin}}}});
    list.list.push_back(tracer::hitable_ptr{new tracer::sphere{glm::vec3{.0,  2., .0}, 2.0, tracer::material_ptr{new tracer::lambertian{marble}}}});

    return list;
}

int main(int argc, char* argv[])
{
    std::string filename("res/output.png");
    // int width, height, channels;

    if (argc > 1) {
        filename = std::string(argv[1]);
    }

    // if (argc > 2) {
    //     width = std::
    // }

    tracer::image image(320, 240, 3);
    int nsamples = 50;
    
    glm::vec3 cameraPos = glm::vec3(13.f, 2.f, 3.f);
    glm::vec3 cameraFront = glm::vec3(0.f, 0.f, 0.f);
    glm::vec3 cameraUp = glm::vec3(0.f, 1.f, 0.f);
    float cameraFov = 20.f;
    float cameraAspect = float(image.get_width()) / float(image.get_height());
    float cameraAperture = 0.0f;
    float cameraFocusDist = 10.f;
    float t0 = 0.0f;
    float t1 = 1.0f;

    tracer::camera camera{cameraPos, cameraFront, cameraUp, cameraFov, cameraAspect, cameraAperture, cameraFocusDist, t0, t1};

    // tracer::hitable_list world = generate_scene();
    tracer::hitable_list world = two_spheres();
    // world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(0.0f, 0.0f, -1.0f), 0.5f,new tracer::lambertian(glm::vec3(0.1f, 0.2f, 0.5f)))});
    // world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(0.0f, -100.5f, -1.0f), 100.0f, new tracer::metal(glm::vec3(0.2f, 0.3f, 0.5f), 0.f))});
    // world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(1.0f, 0.0f, -1.0f), 0.5f, new tracer::metal(glm::vec3(0.8f, 0.6f, 0.2f), 0.1f))});
    // world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f, new tracer::dieletric(1.5f))});
    // world.list.push_back(tracer::hitable_ptr{new tracer::sphere(glm::vec3(-1.0f, 0.0f, -1.0f), -0.45f, new tracer::dieletric(1.5f))});

    // float R = glm::cos(glm::pi<float>() / 4);
    // world.list.push_back(new tracer::sphere(glm::vec3(-R, 0.0f, -1.0f), 0.5f,new tracer::lambertian(glm::vec3(0.f, 0.f, 1.f))));
    // world.list.push_back(new tracer::sphere(glm::vec3( R, 0.0f, -1.0f), 0.5f,new tracer::lambertian(glm::vec3(1.f, 0.f, 0.f))));
    tracer::bvh_node bvh{world.list, t0, t1};

    // for (auto p : world.list) {
    //     std::cout << p << " ";
    // }
    // std::cout << std::endl;

    int r, g, b;
    glm::vec3 col{0.0f};
    tracer::ray ray;

    // std::stringstream stream1, stream2;

    uint32_t image_size = image.get_height() * image.get_width();
    uint32_t current_pixel = 0;
    float percentage = 0;
    // std::string progress = "                                                  ";

    for (uint32_t j = image.get_height() - 1; j < image.get_height(); --j) {
        for (uint32_t i = 0; i < image.get_width(); ++i) {
            col = glm::vec3{0.f};
            for (int n = 0; n < nsamples; ++n) {
                float u = (float(i) + tracer::get_uniform_random()) / float(image.get_width());
                float v = (float(j) + tracer::get_uniform_random()) / float(image.get_height());
                ray = camera.get_ray(u, v);
                col += color(ray, bvh, 0);
            }

            col /= float(nsamples);
            col = glm::sqrt(glm::abs(col));

            r = int(col.r * (float)254.99);
            g = int(col.g * (float)254.99);
            b = int(col.b * (float)254.99);

            image.set_pixel(i, j, glm::ivec3(r,g,b));
            current_pixel++;

            percentage = (float)current_pixel / (float)image_size;

            printf("progress: %.2f%% \r", /*progress.c_str(),*/ percentage * 100);
        }
    }

    image.save(filename, "png");

    // printf("progress: [%s] %.2f\n", progress.c_str(), percentage * 100);
    printf("\ndone. image saved to %s.\n", filename.c_str());

    return 0;
}