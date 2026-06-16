#ifndef CAMERA_H
#define CAMERA_H

#include "RTSphere.h"
#include "vec3.h"
#include "Interval.h"


class Camera {
private:
    vec3 pixel_00;
    vec3 pixel_delta_u;
    vec3 pixel_delta_v;
    vec3 defocus_u, defocus_v;
    Interval ray_interval;
    int image_width, image_height;
    double aspect_ratio;
    int pixel_samples;
    int depth;
    double fov;
    double focus_distance = 10;      //the distance to where we can see the image clearly
    double defocus_angle = 0.6;      //the angle of the camera focus view
    // vectors we need that create an orthonormal basis which is where we position our camera
    vec3 cameraPos = vec3(13,2,3);
    vec3 cameraUp = vec3(0,1,0);
    vec3 cameraAt = vec3(0,0,0);
    //
    vec3 right_vec,up_vec,direction_vec;
    void initialize();
    vec3 ray_color(const Ray& ray, int depth, const Hittable& world);
    Ray get_ray(int i, int j);
    void write_pixel_to_file(ofstream &image_file, const vec3 &pixel_color);
    vec3 defocus_disk_sample();
public:
    Camera(int image_width, double aspect_ration, int depth, int pixel_samples, double fov);
    void render(const Hittable& world);
};



#endif //CAMERA_H
