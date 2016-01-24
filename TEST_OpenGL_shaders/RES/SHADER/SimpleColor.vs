#version 420

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 mModelViewProj;

void main()
{
	fragmentColor = vertexColor;
	gl_Position = mModelViewProj * vec4(vertexPosition, 1);
}