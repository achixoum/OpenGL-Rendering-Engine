#version 330 core
out vec4 FragColor;
uniform vec3 Color;

in vec2 TexCoord;
in vec2 Sphere_Tex_Coord;

void main() {
    FragColor = vec4(Color,1.0f);
}