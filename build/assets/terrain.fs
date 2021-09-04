#version 430
out vec4 FragColor;

in flat uvec4 FragMaterials;
in vec2 FragUV;
in vec3 FragNormal;
uniform sampler2D ImageTexture;	

void main()
{
	vec3 Color = texture(ImageTexture, FragUV).xyz;

	
	FragColor = vec4(Color, 1.0);
} 