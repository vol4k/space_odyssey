#version 430 core

uniform sampler2D colorTexture;

in vec2 vecTex;

out vec4 outColor;
void main()
{
	vec3 textureColor = texture(colorTexture, vecTex).xyz;
	outColor = vec4(textureColor, 1.0);
}