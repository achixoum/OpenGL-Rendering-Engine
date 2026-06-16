#include "Camera.h"
#include "Material.h"

// We need only the width of the image and the preferred aspect ratio of the image
// so we pass them in the initialization of the camera
Camera::Camera(int image_width, double aspect_ratio, int depth, int pixel_samples, double fov) {
    this->image_width = image_width;
    this->aspect_ratio = aspect_ratio;
    this->pixel_samples = pixel_samples;
    this->depth = depth;
    this->fov = degrees_to_radians(fov);
    initialize();
}

void Camera::initialize() {
    image_height =  int (image_width/aspect_ratio);
    image_height = image_height<1 ? 1:image_height;

    image_file <<"P3"<< endl;
    image_file << image_width <<" "<< image_height<<endl;
    image_file <<"255"<< endl;

    //camera
    double h = tan(fov/2);
    double focal_length = focus_distance;
    double viewport_height = 2.0 * h * focal_length;
    double viewport_width = viewport_height*((double)image_width/image_height);

    direction_vec = unit_vector(cameraPos-cameraAt);
    right_vec = unit_vector(cross(cameraUp, direction_vec));
    up_vec = cross(direction_vec,right_vec);

    //ray interval, indicates how far the ray can intersect
    ray_interval = Interval(0.001,infinity);
    //vectors that are helping navigate through the viewport grid

    vec3 vu = viewport_width*right_vec;
    vec3 vv = viewport_height*(-1.0*up_vec);

    //vectors that will navigate us from pixel to pixel since the resolution of the image is
    //image_width/image_height(600 x 337) and the virtual viewport is viewport_width/viewport_height(2.0 x 1.125)
    //the scale from pixel to pixel will be viewport_resolution/image_resolution/
    pixel_delta_u = vu/image_width;
    pixel_delta_v = vv/image_height;

    //compute the top left position of the viewport and the (0,0) pixel of the grid
    vec3 top_left_pos = cameraPos - (focal_length*direction_vec) - vu/2 - vv/2;
    pixel_00 = top_left_pos + 0.5*(pixel_delta_u + pixel_delta_v);

    double defocus_radius = focus_distance*tan(degrees_to_radians(defocus_angle/2));
    defocus_u = right_vec*defocus_radius;
    defocus_v = up_vec*defocus_radius;
}

vec3 Camera::ray_color(const Ray& ray, int depth, const Hittable &world) {
    if (depth <= 0)
        return vec3(0,0,0);

    hit_record record;
    if (world.hit(ray, ray_interval, record)) {
        Ray scattered;
        vec3 attenuation;
        if (record.material_type->scatter(ray, record, attenuation, scattered)) {
            return attenuation * ray_color(scattered, depth-1, world);
        }
        return vec3(0,0,0);
    }

    vec3 unit_direction = unit_vector(ray.getDirection());
    double a = (unit_direction.getY()+1.0)*0.5;
    return (1.0-a)*vec3(1.0,1.0,1.0) + a*vec3(0.5, 0.7, 1.0);
}

Ray Camera::get_ray(int i, int j) {

    //offset creates a vec2 squared interval [-.5,-.5] - [+.5,+.5] that will generate
    //that we will use it to sample randomly around the current pixel position(i,j)
    vec3 offset = vec3(random_double() - 0.5, random_double() + 0.5, 0);

    vec3 pixel_location = pixel_00 + (i+offset.getX())*pixel_delta_u + (j+offset.getY())*pixel_delta_v;

    vec3 origin = (defocus_angle <=0) ? cameraPos : defocus_disk_sample();

    vec3 ray_direction = pixel_location - cameraPos;

    // create the ray that its origin is the camera position which it intersects towards,
    // the current pixel location
    return {Ray(origin, ray_direction)};
}

vec3 Camera::defocus_disk_sample() {
    vec3 r = random_in_unit_disk();
    return cameraPos + (r.getX()*defocus_u) + (r.getY()*defocus_v);
}


// Writes a color(pixel_color) to a specific pixel of our image
// pixel_color must be a unit vector(length of the vector = 1)
void Camera::write_pixel_to_file(ofstream &image_file, const vec3 &pixel_color) {
    double r = linear_to_gamma(pixel_color.getX());
    double g = linear_to_gamma(pixel_color.getY());
    double b = linear_to_gamma(pixel_color.getZ());


    Interval unit_vector_interval(0.000, 0.999);
    int ir = int(256 * unit_vector_interval.clamp(r));
    int ig = int(256 * unit_vector_interval.clamp(g));
    int ib = int(256 * unit_vector_interval.clamp(b));

    image_file << ir << " " << ig << " " << ib <<" ";
}

void Camera::render(const Hittable& world) {
    for (int j=0; j<image_height; j++) {
        for (int i=0; i<image_width; i++) {
            vec3 pixel_color = vec3(0,0,0);
            for (int k=0; k<pixel_samples; k++) {
                //current ray is the computed ray from the sampled pixel around the pixel(i,j)
                Ray current_ray = get_ray(i,j);
                pixel_color += ray_color(current_ray, depth, world);
            }
            write_pixel_to_file(image_file, pixel_color/pixel_samples);
        }
        image_file << endl;
    }
}
