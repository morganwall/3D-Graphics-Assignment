#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normals;
layout(location = 2) in vec2 textureCoords;
layout(location = 3) in vec3 tangent;
layout(location = 4) in vec3 bitangent;

out vec3 vertPos;
out vec3 vertNormals;
out vec2 vertTextureCoords;
out mat3 vertTBN; // Tangent, Bitangent, Normals matrix.

// Matrices.
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);

	vertPos = vec3(model * vec4(position, 1.0f));
	vertNormals = mat3(transpose(inverse(model))) * normals;
	vertTextureCoords = textureCoords;
	// Get Tangent, Bitangent, and Normals in World Space.
	vertTBN = mat3(normalize(mat3(model) * tangent), normalize(mat3(model) * bitangent), normalize(mat3(model) * normals));
}