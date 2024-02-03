#version 430 core

uniform sampler2D colorTexture;
uniform sampler2D clouds;

uniform vec3 sunPos;
uniform vec3 sunColor;
uniform float sunLightExp;

uniform vec3 cameraPos;

uniform vec3 reflectorPos;
uniform vec3 reflectorDir;
uniform vec3 reflectorColor;
uniform float reflectorAngle;
uniform float reflectorLightExp;

vec3 normalizedVertexNormal;

in vec3 vertexNormalOut;
in vec3 vertexPosOut;
in vec2 vertexTexCoordOut;

out vec4 outColor;

vec4 calcPointLight(vec3 fragColor, vec3 lightPos, vec3 lightColor, float lightExp) {
	vec3 lightDir = normalize(vertexPosOut - lightPos);
	float lightDistance = length(vertexPosOut - lightPos);
	vec3 newLightColor = lightColor / pow(lightDistance, 2);

	float intensity = dot(normalizedVertexNormal, -lightDir);
	intensity  = max(intensity, 0.0);
	
	vec3 viewDir = normalize(cameraPos - vertexPosOut);
	vec3 reflectDir = reflect(lightDir, normalizedVertexNormal);
	
	float glossPow = 8;
	float specular = pow(max(dot(viewDir, reflectDir), 0.0), glossPow);

	float diffuse = intensity;
	vec3 resultColor = newLightColor * (fragColor * diffuse + specular );
	return vec4(1 - exp(-resultColor  * lightExp), 1.0);
}

vec4 calcSpotLight(vec3 fragColor, vec3 lightPos, vec3 lightColor, vec3 lightDir,
				   float innerCutOff, float outerCutOff, float lightExp) {
	vec3 lightToFragDir = normalize(vertexPosOut - lightPos);
	float angleCos = dot(lightToFragDir, lightDir);
	float epsilon = cos(innerCutOff) - cos(outerCutOff);
	vec4 res = vec4(0, 0, 0, 1);
	if (angleCos > cos(outerCutOff)) {
		float intensity = clamp((angleCos - cos(outerCutOff)) / epsilon, 0.0, 1.0);  
		res = calcPointLight(fragColor, lightPos, lightColor, reflectorLightExp  * intensity);
	}
	return res;
}


void main()
{
	normalizedVertexNormal = normalize(vertexNormalOut);
	vec3 textureColor = texture(colorTexture, vertexTexCoordOut).rgb;
	vec3 cloudColor = texture(clouds, vertexTexCoordOut).rgb;

	textureColor = mix(vec3(1), textureColor, cloudColor.r);
	
	normalizedVertexNormal = normalize(vertexNormalOut);

	outColor = calcPointLight(textureColor, sunPos, sunColor, sunLightExp);

	outColor += calcSpotLight(textureColor, reflectorPos, reflectorColor, reflectorDir, 
							  reflectorAngle, reflectorAngle + radians(10), reflectorLightExp);
	
}