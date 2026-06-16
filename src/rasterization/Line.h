#ifndef LINE_H
#define LINE_H

#include <vector>
#include <glad/glad.h>
#include "Shader.h"
#include "../../glm/glm.hpp"

using namespace std;
using namespace glm;

class Line {
private:
    vec3 vertices[2];
    unsigned int VBO, VAO;
    Shader lineShader;
    vec3 color;
public:
    Line(vec3 start, vec3 end, const char* vertex, const char* fragment);
    ~Line();
    void setColor(vec3 color);
    void setAttributes();
    void draw(mat4 model, mat4 view, mat4 projection);
};



#endif