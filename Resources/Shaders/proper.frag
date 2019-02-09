#version 130

in vec3 fPos;
in vec2 fUV;
in vec3 fNorm;

out vec4 fCol;

struct Light {
    //-----------------------------------------
    // Because there are no enums inside GLSL,
    // an int has to be used in its place.
    //-----------------------------------------
    // 0 - Directional light
    // 1 - Point light
    // 2 - Spotlight
    //-----------------------------------------
    int type;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 pos; // Position, applies to 1 & 2
    vec3 dir; // Direction, applies to 0 & 2

    float cutoff; 	   // Inner cutoff angle, applies only to 2
    float outerCutoff; // Outer cutoff angle, applies only to 2

    float constAtt; // Constant  attenuation, always 1.0
    float linAtt;   // Linear 	 attenuation, should be 0.0 for type 0
    float quadAtt;  // Quadratic attenuation, should be 0.0 for type 0
};

uniform vec3 camPos;

uniform Light lights[8];
uniform int activeLights;

// A struct for a material is useless,
// so seperate uniforms are used
uniform sampler2D matDiffuse;
uniform vec3	  matSpecular;
uniform bool      asdf;
uniform float 	  matShininess = 0.0;

void main(){
    fCol = vec4(0, 0, 0, 1);

    for(int i = 0; i < activeLights; i++){
	// ----- Ambient -----
	vec3 AMBIENT = texture2D(matDiffuse, fUV).rgb * lights[i].ambient;

	// ----- Diffuse ------
	vec3 lightDir; // Direction from fragment to light
	float lightDist;
	float intensity = 1.0;

	if(lights[i].type == 0){
	    lightDir = -lights[i].dir;
	    lightDist = 0.0;
	} else if(lights[i].type == 1){
	    lightDir = normalize(lights[i].pos - fPos);
	    lightDist = length(lights[i].pos - fPos);
	} else if(lights[i].type == 2) {
	    lightDir = normalize(lights[i].pos - fPos);
	    lightDist = length(lights[i].pos - fPos);
	    float theta = dot(lightDir, normalize(-lights[i].dir));
	    float epsilon = lights[i].cutoff - lights[i].outerCutoff;
	    intensity = clamp((theta - lights[i].outerCutoff) / epsilon, 0.0, 1.0);
	}

	float attenuation = 1.0 /
	    (lights[i].constAtt +
	     lights[i].linAtt * lightDist +
	     lights[i].quadAtt * (lightDist * lightDist));

	float diff = max(dot(fNorm, lightDir), 0.0);
	vec3 DIFFUSE = lights[i].diffuse * (diff * texture2D(matDiffuse, fUV)).rgb;

	// ----- Specular -----
	vec3 camDir = normalize(camPos - fPos);
	vec3 reflectDir = reflect(-lightDir, fNorm);
	float spec = pow(max(dot(camDir, reflectDir), 0.0), matShininess);
	vec3 SPECULAR = lights[i].specular * (spec * matSpecular);

	AMBIENT *= attenuation * intensity;
	DIFFUSE *= attenuation * intensity;
	SPECULAR *= attenuation * intensity;

	fCol.rgb += AMBIENT
	    + DIFFUSE
	    + SPECULAR
	    ;
    }
}
