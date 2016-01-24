#version 420

in vec3 fragmentColor;

out vec4 color;

uniform float fAlpha;

void main()
{
	color = vec4(fragmentColor, fAlpha);
}