#version 130

in vec3 pos;
in vec2 UV;

out vec2 fUV;

uniform mat4 transform;
uniform mat4 UVtransform;

void main(){
	gl_Position = transform * vec4(pos, 1);
	gl_PointSize = 5;

	fUV = (UVtransform * vec4(UV, 0, 1)).xy;
}
