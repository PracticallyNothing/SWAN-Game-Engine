#version 130

in vec2 pos;
in vec2 UV;

out vec2 fUV;

uniform vec2 offset;

void main(){
	gl_Position = vec4(pos + offset, 0, 1);
	gl_PointSize = 5;

	fUV = UV;
}
