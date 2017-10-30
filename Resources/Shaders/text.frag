#version 130

in vec2 fUV;

out vec4 fCol;

uniform sampler2D tex;

void main(){
	fCol = texture2D(tex, vec2(fUV.x, fUV.y));
}
