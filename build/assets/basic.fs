#version 430

layout(location = 3) uniform vec3 Color;

out vec4 FragColor;

void main()
{
	FragColor = vec4(1,1,0,1);
	float Sum = Color.x + Color.y + Color.z;
	if(Sum > 0)
		FragColor = vec4(Color, 1);
}