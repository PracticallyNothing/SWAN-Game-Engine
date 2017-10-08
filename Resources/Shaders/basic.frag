#version 130

in vec2 fUV;
out vec4 fCol;

uniform sampler2D diffuse;

void main() { fCol = texture2D(diffuse, vec2(fUV.x, -fUV.y)); }
