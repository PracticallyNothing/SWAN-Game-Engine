#version 130

in vec2 _UV;

out vec4 fCol;

uniform sampler2D diffuse;
uniform float opacity;

void main(){
	fCol = clamp(opacity, 0.0, 1.0) * texture2D(diffuse, vec2(_UV.x, -_UV.y));
}
