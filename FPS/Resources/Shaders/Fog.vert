#version 130

in vec3 pos;
in vec2 UV;

out float fogPower;
out vec2 fUV;

uniform float fogZ;
uniform mat4 perspective;
uniform mat4 view;
uniform mat4 transform;

float Normalize(float V, float Min, float Max) { return (V - Min) / (Max - Min); }

void main() {
    gl_Position = perspective * view * transform * vec4(pos, 1);
    gl_PointSize = 20;

	fogPower = clamp(Normalize(gl_Position.z, fogZ, 0), 0, 1);
    fUV = UV;
}

