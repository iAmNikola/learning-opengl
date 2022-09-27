#version 330 core
in vec3 vertex_color;
in vec2 vertex_texture;

out vec4 FragColor;

uniform sampler2D my_texture;

void main() { FragColor = texture(my_texture, vertex_texture) * vec4(vertex_color, 0.6); }