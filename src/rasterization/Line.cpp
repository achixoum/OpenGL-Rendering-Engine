#include "Line.h"

Line::Line(vec3 start, vec3 end, const char* vertex, const char* fragment): lineShader(vertex, fragment) {
    vertices[0].x = start.x;
    vertices[0].y = start.y;
    vertices[0].z = start.z;
    vertices[1].x = end.x;
    vertices[1].y = end.y;
    vertices[1].z = end.z;
    color = vec3(1.0f,1.0f,1.0f);
}

Line::~Line() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    lineShader.deleteVertexShader();
    lineShader.deleteFragmentShader();
    lineShader.deleteProgram();
}

void Line::setColor(vec3 color) {
    this->color = color;
}

void Line::setAttributes() {
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vec3)*2, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Line::draw(mat4 model, glm::mat4 view, glm::mat4 projection) {

    lineShader.use();
    lineShader.setVec3("Color", vec3(this->color));


    lineShader.setMat4("model", model);
    lineShader.setMat4("view", view);
    lineShader.setMat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_LINES,0, 2);
    glBindVertexArray(0);
}