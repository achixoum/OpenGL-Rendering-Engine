#include <chrono>
#include "util.h"
#include "RTSphere.h"
#include "Hittable.h"
#include "Hittable_list.h"
#include "Camera.h"

#define IMAGE_WIDTH 1280
constexpr double aspect_ratio = 16.0/9.0;
ofstream image_file;


using namespace std;


int main() {
    image_file = ofstream("C:/OpenGL/Assets/Images/image.ppm");
    auto start = chrono::high_resolution_clock::now();
    //world objects
    Hittable_list world;
    auto ground_material = make_shared<lambertian>(vec3(0.5, 0.5, 0.5));
    world.add_object(make_shared<RTSphere>( 1000,vec3(0,-1000,0), ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            vec3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = vec3::random_vec3() * vec3::random_vec3();
                    sphere_material = make_shared<lambertian>(albedo);
                    world.add_object(make_shared<RTSphere>( 0.2,center, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = vec3::random_vec3(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add_object(make_shared<RTSphere>( 0.2,center, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add_object(make_shared<RTSphere>( 0.2,center, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add_object(make_shared<RTSphere>( 1.0,vec3(0, 1, 0), material1));

    auto material2 = make_shared<lambertian>(vec3(0.4, 0.2, 0.1));
    world.add_object(make_shared<RTSphere>( 1.0,vec3(-4, 1, 0), material2));

    auto material3 = make_shared<Metal>(vec3(0.7, 0.6, 0.5), 0.0);
    world.add_object(make_shared<RTSphere>( 1.0,vec3(4, 1, 0), material3));

    Camera camera(IMAGE_WIDTH, aspect_ratio,20, 20,20.0);

    camera.render(world);

    auto end = chrono::high_resolution_clock::now();

    // Calculate duration
    auto duration = duration_cast <chrono::microseconds > (end - start);
    cout<<duration.count()/60000000<<" seconds"<<endl;

    image_file.close();
    return 0;
}
