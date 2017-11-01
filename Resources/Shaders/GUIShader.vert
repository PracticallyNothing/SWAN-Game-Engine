
#version 130

in vec3 pos;
in vec2 UV;

out vec2 _pos;
out vec2 _UV;

uniform mat4 transform;

void main() {
	gl_Position = transform * vec4(pos.x, pos.y, 0, 1);

	_pos = gl_Position.xy;
	_UV  = UV;
}
