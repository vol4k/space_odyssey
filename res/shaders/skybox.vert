#version 430 core

layout(location = 0) in vec3 vertexPosition;

uniform mat4 transformation;

out vec3 texCoord;

void main()
{
    vec4 pos = transformation * vec4(vertexPosition, 1.0);
    gl_Position = pos.xyww;
    texCoord = vertexPosition;
}