#version 130

in vec3 pos;
in vec2 UV;

out vec2 fUV;

uniform vec2 UVScale;
uniform vec2 UVOffset;

uniform mat4 model;

void main(){
	gl_Position = model * vec4(pos, 1);
	gl_PointSize = 5;

	fUV = (UV * UVScale) + UVOffset;
}
