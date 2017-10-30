#version 130

in vec2 fUV;

out vec4 fCol;

uniform vec4 color = vec4(0, 0, 0, 1);
uniform vec4 bgColor = vec4(0, 0, 0, 0);
uniform sampler2D tex;

void main(){
	fCol = vec4(color.rgb, texture2D(tex, vec2(fUV.x, fUV.y)).a);

	fCol = mix(bgColor, fCol, fCol.a);
}
