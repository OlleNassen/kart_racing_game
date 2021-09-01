#version 430

layout(location = 0) in vec3 LocalPosition;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;

layout(location = 0) uniform mat4 Model;
layout(location = 1) uniform mat4 View;
layout(location = 2) uniform mat4 Projection;

void main()
{
	gl_Position = Projection * View * Model * vec4(LocalPosition, 1.0);
}