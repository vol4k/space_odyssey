#version 430 core

uniform float exposition;
uniform sampler2D diffuseTexture;
uniform sampler2D normalTexture;

in vec2 texCoords;

out vec4 outColor;

void main()
{
	vec3 textureColor = texture(diffuseTexture, texCoords).xyz;
	outColor = vec4(vec3(1.0) - exp(-textureColor*exposition),1);
}