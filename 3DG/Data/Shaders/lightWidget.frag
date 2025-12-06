#version 460 core

in vec2 texturePos;

uniform vec3 baseColor;
uniform int lightType;

out vec4 fragColor; // Output colour.

void main()
{
	fragColor = vec4(baseColor, 1.0f);
}