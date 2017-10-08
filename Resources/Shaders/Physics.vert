#version 130

in vec3 pos;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 transform;

void main() { gl_Position = perspective * view * transform * vec4(pos, 1); }
