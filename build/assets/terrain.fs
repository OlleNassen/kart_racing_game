#version 430
out vec4 FragColor;

in flat uvec4 FragMaterials;
in vec2 FragUV;
in vec3 FragNormal;
uniform sampler2DArray ImageTexture;	

void main()
{
	vec3 X0Z0 = texture(ImageTexture, vec3(FragUV, FragMaterials.x)).xyz;
	vec3 X1Z0 = texture(ImageTexture, vec3(FragUV, FragMaterials.y)).xyz;
	vec3 X0Z1 = texture(ImageTexture, vec3(FragUV, FragMaterials.z)).xyz;
	vec3 X1Z1 = texture(ImageTexture, vec3(FragUV, FragMaterials.w)).xyz;
		
	vec3 Color = vec3(1);

	
	FragColor = vec4(Color, 1.0);
} 