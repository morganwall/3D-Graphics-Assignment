#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 normals;

// Matrices.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Output. To be passed to the fragment shader.
out vec3 vertPos;
out vec2 vertUV;
out vec3 vertNormals;

void main()
{
	// Model Space Pos -> World Space Pos.
	vertPos = vec3(model * vec4(position, 1.0f));
	vertUV = uv;
	// Get Normals in World Space.
	vertNormals = mat3(transpose(inverse(model))) * normals;

	gl_Position = projection * view * vec4(vertPos, 1.0f);
}