#version 430
layout (location = 0) in vec3 Position;
layout (location = 1) in uvec4 Materials;

out flat uvec4 FragMaterials;
out vec2 FragUV;
out vec3 FragNormal;

layout(location = 0) uniform mat4 Model;
layout(location = 1) uniform mat4 View;
layout(location = 2) uniform mat4 Projection;

void main()
{
	FragMaterials = Materials;
	FragUV = Position.xz;
	FragNormal = vec3(1.0, 0.0, 0.0);
	gl_Position = Projection * View * Model * vec4(Position, 1.0);
}