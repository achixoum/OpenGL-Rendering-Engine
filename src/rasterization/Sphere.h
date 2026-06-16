#ifndef SHPERE_H
#define SHPERE_H

#include "../../glm/glm.hpp"

using namespace glm;
using namespace std;


class Sphere {
    private:
        float radius;
        unsigned int VBO;
        unsigned int VAO;
        unsigned int EBO;
        vector<vec3> vertices;
        vector<unsigned int> indices;
        vector<vec2> tex_coordinates;
        vec3 center;
        vector<float> data;
        vector<vec3> normals;
    public:
        Sphere(float radius, vec3 center);
        ~Sphere();
        void createVertices(int stacks, int sectors);
        void setAttributes();
        void createBuffers();
        void processAttribForBuffer();
        void draw();
        void printProperties();
        vec3 getPos();
};


#endif //SHPERE_H
