#version 430 core

uniform sampler2D colorTexture;

in vec2 vecTex;

out vec4 outColor;
void main()
{
	vec3 textureColor = texture(colorTexture, vecTex).xyz;
	float alpha = smoothstep(0.1, 0.2, vecTex.y);
	outColor = vec4(textureColor, alpha);
}