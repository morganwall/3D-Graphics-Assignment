#version 460 core

layout(location = 0) in vec3 position;

// Matrices.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texturePos;

void main()
{
	// Get Texture Position.
	texturePos = position.xy + 0.5f; // Add 0.5f to convert from vertex position to texture position.

	gl_Position = projection * view * model * vec4(position, 1.0f);
}