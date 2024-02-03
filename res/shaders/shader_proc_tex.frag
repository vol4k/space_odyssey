#version 430 core

uniform sampler2D colorTexture;
uniform sampler2D rust;
uniform sampler2D scratches;

uniform vec3 sunPos;
uniform vec3 sunColor;
uniform float sunLightExp;

uniform vec3 cameraPos;
uniform float time;

uniform vec3 reflectorPos;
uniform vec3 reflectorDir;
uniform vec3 reflectorColor;
uniform float reflectorAngle;
uniform float reflectorLightExp;

vec3 normalizedVertexNormal;

in vec3 vertexNormalOut;
in vec3 vertexPosOut;
in vec2 vertexTexCoordOut;
in vec3 vertexLocPos;

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

vec4 calcSpotLight(vec3 fragColor, vec3 lightPos, vec3 lightColor, vec3 lightDir, float lightExp) {
	vec3 reflectorLightDir = normalize(vertexPosOut - reflectorPos);
	float angleCos = dot(reflectorLightDir, reflectorDir);
	float reflectorOutAngle = reflectorAngle + radians(10);
	float epsilon = cos(reflectorAngle) - cos(reflectorOutAngle);
	vec4 res = vec4(0, 0, 0, 1);
	if (angleCos > cos(reflectorOutAngle)) {
		float intensity = clamp((angleCos - cos(reflectorOutAngle)) / epsilon, 0.0, 1.0);  
		res = calcPointLight(fragColor, reflectorPos, reflectorColor, reflectorLightExp  * intensity);
	}
	return res;
}


void main()
{
	vec3 shipColor = texture2D(colorTexture, vertexTexCoordOut).rgb;
	vec3 rustColor = texture2D(rust, vertexTexCoordOut).rgb;
	vec3 scratchesColor = texture2D(scratches, vertexTexCoordOut).rgb;
	vec3 textureColor = mix(rustColor, shipColor, scratchesColor .r);

	if (sin(vertexLocPos.y * vertexLocPos.x * vertexLocPos.z) > 0) {
		textureColor = vec3(1, 0, 0);
	}
	
	normalizedVertexNormal = normalize(vertexNormalOut);

	outColor = calcPointLight(textureColor, sunPos, sunColor, sunLightExp);

	vec3 reflectorLightDir = normalize(vertexPosOut - reflectorPos);
	float angleCos = dot(reflectorLightDir, reflectorDir);
	float reflectorOutAngle = reflectorAngle + radians(10);
	float epsilon = cos(reflectorAngle) - cos(reflectorOutAngle);
	if (angleCos > cos(reflectorOutAngle)) {
		float intensity = clamp((angleCos - cos(reflectorOutAngle)) / epsilon, 0.0, 1.0);  
		outColor += calcSpotLight(textureColor, reflectorPos, reflectorColor, reflectorLightDir, reflectorLightExp  * intensity);
	}
}

