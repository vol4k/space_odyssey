#version 430 core

uniform samplerCube skybox;
uniform float exposition;

in vec3 texCoord;

out vec4 outColor;

void main()
{
	vec3 textureColor = texture(skybox, texCoord).xyz;
	outColor = vec4(vec3(1.0) - exp(-textureColor*exposition),1);
}