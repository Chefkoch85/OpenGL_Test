#version 420

in vec3 vertexPosition;
in vec3 vertexColor;

out vec3 fragmentColor;

uniform mat4 mModelViewProj;

void main()
{
	fragmentColor = vertexColor;
	gl_Position = mModelViewProj * vec4(vertexPosition, 1);
}