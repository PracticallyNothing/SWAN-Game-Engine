#version 330

in vec3 pos;
in vec2 UV;
in vec3 norm;

out vec3 fPos;
out vec2 fUV;
out vec3 fNorm;

uniform mat4 perspective;
uniform mat4 view;
uniform mat4 transform;

void main () {
	gl_Position = perspective *
				  view *
				  transform *
				  vec4(pos, 1);
	gl_PointSize = 20;


	fPos = (transform * vec4(pos, 1)).xyz;
	fUV = UV;
	fNorm = normalize(mat3(transpose(inverse(transform))) * norm);
}

