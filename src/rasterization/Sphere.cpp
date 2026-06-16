#include "Sphere.h"
#include <cmath>
#include <glad/glad.h>
#include <iostream>

Sphere::Sphere(float radius, vec3 center) {
    this->radius = radius;
    this->center = center;
}

Sphere::~Sphere() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void Sphere::createVertices(int stacks, int sectors) {
    vertices.clear();
    indices.clear();

    for (int y=0; y<=stacks; y++) {
        //float v = (float)y / (float)stacks;
        //float f_angle = M_PI * (v - 0.5f); // -PI/2 → PI/2
        for (int x=0; x<=sectors; x++) {
            //float u = (float)x / (float)sectors;
            //float theta_angle = u * 2.0f * M_PI;
            //θ and φ angles of a corresponding sector
            float f_angle     = M_PI/2.0f - M_PI*((float)y/stacks);
            float theta_angle = 2.0f*M_PI*((float)x/sectors);

            //compute the position using the parametric equations of a sphere
            float xPos = center.x + radius * cos(f_angle) * cos(theta_angle);
            float yPos = center.y + radius * sin(f_angle);
            float zPos = center.z + radius * sin(theta_angle) * cos(f_angle);

            //compute the texture coordinates by applying linear mapping (Latitude(φ)/Longitude(Θ))
            double u,v;

            v = (float)y/(float)stacks;
            u = (float)x/(float)sectors;

            vertices.emplace_back(xPos, yPos, zPos);
            tex_coordinates.emplace_back(static_cast<float>(u),static_cast<float>(v));
            normals.emplace_back(xPos, yPos, zPos);
            //cout<<"("<<xPos<<", "<<yPos<<", "<<zPos<<", "<<u<<", "<<v<<")"<<endl;
        }
    }


    for (int y=0; y<stacks; y++) {
        for (int x=0; x<sectors; x++) {
            int indexA = y * (sectors + 1) + x;
            int indexB = y * (sectors + 1) + x + 1;
            int indexC = (y + 1) * (sectors + 1) + x;
            int indexD = (y + 1) * (sectors + 1) + x + 1;

            indices.push_back(indexA);
            indices.push_back(indexB);
            indices.push_back(indexC);

            indices.push_back(indexB);
            indices.push_back(indexD);
            indices.push_back(indexC);

            //cout<<"("<<indexA<<", "<<indexB<<", "<<indexC<<")"<<endl;
        }
    }
}

void Sphere::setAttributes() {
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(data.size() * sizeof(float)), &data[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)), &indices[0], GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(3*sizeof(float)));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2,2,GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(sizeof(float)*6));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void Sphere::processAttribForBuffer() {
    for (unsigned int i=0; i<vertices.size(); i++) {
        data.push_back(vertices[i].x);
        data.push_back(vertices[i].y);
        data.push_back(vertices[i].z);

        data.push_back(normals[i].x);
        data.push_back(normals[i].y);
        data.push_back(normals[i].z);

        if (!tex_coordinates.empty()) {
            data.push_back(tex_coordinates[i].x);
            data.push_back(tex_coordinates[i].y);
        }
    }
}

void Sphere::printProperties() {
    cout<<"Vertices count: "<<vertices.size()<<endl;
    cout<<"Indices count: "<<indices.size()<<endl;
    cout<<"Normals count: "<<normals.size()<<endl;
}

void Sphere::createBuffers() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

void Sphere::draw() {
    glBindVertexArray(VAO);

    glDrawElements(GL_TRIANGLES, indices.size(),GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

vec3 Sphere::getPos() {
    return center;
}