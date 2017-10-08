#version 330

in vec3 fPos;
in vec2 fUV;
in vec3 fNorm;

out vec4 fCol;

struct DirectionalLight { 
	vec3 dir; 
	vec3 color; 
};

struct PointLight { 
	vec3 pos; 
	float rad; 
	vec3 color; 
};

uniform sampler2D diffuse;
uniform vec3 ambient;

uniform DirectionalLight sun;

uniform int activePointLights;
uniform PointLight pointLights[8];

float Normalize(float v, float fMin, float fMax){
	return (v - fMin) / (fMax - fMin);
}

void main(){
	vec4 color = clamp(dot(-sun.dir, fNorm), 0.0, 1.0) *
				 vec4(sun.color, 1) *
				 texture2D(diffuse, fUV);
	
	for(int i = 0; i < activePointLights; i++){
		vec3 lightNorm = normalize(pointLights[i].pos - fPos);
		float lightDist = length(pointLights[i].pos - fPos);

		color += 
			// Check if the pixel is facing the light
				clamp(dot(fNorm, lightNorm), 0.0, 1.0) *
			// Check if the light and the pixel are close enough
				clamp(Normalize(lightDist, pointLights[i].rad, 0.0), 0.0, 1.0) *
			// Get the colour of the light
				vec4(pointLights[i].color, 1) *
			// Get the colour of the pixel
				texture2D(diffuse, vec2(fUV.x, -fUV.y));
	}
	
	fCol = max(vec4(ambient, 1), color);
}
