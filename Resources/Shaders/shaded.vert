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
uniform mat4 rotation;

void main(){
	gl_Position = perspective * 
				  view * 
				  transform * 
				  vec4(pos, 1);
	gl_PointSize = 20;
	
	
	fPos = (transform * vec4(pos, 1)).xyz;
	fUV = UV;
	fNorm = (rotation * vec4(norm, 1)).xyz;
}

