#version 130

in vec2 _UV;

out vec4 fCol;

uniform sampler2D diffuse;

void main(){
	fCol = texture2D(diffuse, vec2(_UV.x, -_UV.y));
}
