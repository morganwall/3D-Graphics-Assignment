#version 460 core

uniform vec3 baseColor;

out vec4 fragColor; // Output colour.

void main()
{
	fragColor = vec4(baseColor, 1.0f);
}