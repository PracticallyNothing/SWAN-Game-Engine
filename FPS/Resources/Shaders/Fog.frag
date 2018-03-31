#version 130

in float fogPower;
in vec2 fUV;
out vec4 fCol;

uniform vec3 fogColor;
uniform sampler2D diffuse;

float Normalize(float V, float Min, float Max) { return (V - Min) / (Max - Min); }

void main() {
	fCol = mix(vec4(fogColor, 1), texture2D(diffuse, vec2(fUV.x, -fUV.y)), fogPower);
}
