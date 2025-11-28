#version 460 core

uniform samplerCube skybox;
uniform int skyboxMode;
uniform vec3 skyboxColour;

in vec3 vertTexturePos;

out vec4 fragColor; // Output colour.

void main()
{
	// Check Skybox Mode.
	if (skyboxMode == 0) // Cubemap.
		fragColor = texture(skybox, vertTexturePos);
	else
		fragColor = vec4(skyboxColour, 1.0f);
}