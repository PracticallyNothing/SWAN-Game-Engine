#version 130

in vec2 fUV;

out vec4 fCol;

uniform vec3 color;
uniform sampler2D tex;

void main(){
	fCol = vec4(color, texture2D(tex, vec2(fUV.x, -fUV.y)).a);
}
