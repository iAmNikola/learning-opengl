#version 330 core
layout (location = 0) in vec3 pos;
//layout (location = 1) in vec3 color;
layout (location = 1) in vec2 in_texture;

//out vec3 vertex_color;
out vec2 vertex_texture;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 transform;

void main() {
    gl_Position = projection * view * transform * model * vec4(pos.x, pos.y, pos.z, 1.0);
    //vertex_color = color;
    vertex_texture = in_texture;
}
