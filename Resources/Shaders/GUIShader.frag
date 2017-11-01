#version 130

in vec2 _pos;
in vec2 _UV;

out vec4 fCol;

uniform vec2 topLeft;
uniform vec2 bottomRight;

uniform int type;
uniform vec4 color1;
uniform vec4 color2;
uniform sampler2D tex;

float Normalize(float V, float Min, float Max) {
	return (V - Min) / (Max - Min);
}

void main() {
	if(type == 0) // T_COLOR
		fCol = color1;
	else if(type == 1) // T_VERTGRAD
		mix(color1, color2, Normalize(_pos.y, topLeft.y, bottomRight.y));
	else if(type == 2) // T_HORIZGRAD
		mix(color1, color2, Normalize(_pos.x, topLeft.x, bottomRight.y));
	else if(type == 3) // T_TEXTURE
		fCol = texture2D(tex, vec2(_UV.x, -_UV.y));
}
