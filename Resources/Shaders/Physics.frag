#version 130

out vec4 fCol;

uniform int colliding;

void main() {
	fCol = vec4(1, 0, 0, 1) - colliding * vec4(1, -1, 0, 0);
}
