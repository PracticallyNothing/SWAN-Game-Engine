#version 130

in vec3 pos;
in vec2 UV;

out vec2 fUV;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 transform;

void main() {
    gl_Position = perspective * view * transform * vec4(pos, 1);
    gl_PointSize = 20;

    fUV = UV;
}
