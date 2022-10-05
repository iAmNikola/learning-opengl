#version 330 core
//in vec3 vertex_color;
in vec2 vertex_texture;

out vec4 FragColor;

uniform sampler2D my_texture0;
uniform sampler2D my_texture1;

void main() {
    FragColor = mix(
        texture(my_texture0, vertex_texture),
        texture(my_texture1, vertex_texture),
        0.3
    );
}