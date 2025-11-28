#version 460 core

layout(location = 0) in vec3 position;

// Matrices.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Output. To be passed to the fragment shader.
out vec3 vertTexturePos;

void main()
{
	vertTexturePos = position;
	gl_Position = (projection * view * vec4(vertTexturePos, 1.0f)).xyww;
}