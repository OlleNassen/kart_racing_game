#version 430
out vec4 FragColor;

in flat uvec4 FragMaterials;
in vec2 FragUV;
in vec3 FragNormal;
in vec2 WorldPos;
layout (location = 3) uniform sampler2D ImageTexture;	
layout (location = 4) uniform sampler2D ImageTexture2;

layout(location = 5) uniform int NumPoints;
layout(location = 6) uniform vec2 Points[512];


float dist2(vec2 a, vec2 b)
{
    vec2 p = a - b;
    return dot(p, p);
}

void main()
{
	vec3 Color = texture(ImageTexture, FragUV * vec2(0.1)).xyz;
    
    for (int i = 0; i < NumPoints; ++i)
    {
        if (dist2(WorldPos, Points[i]) < 10.0)
        {
            Color = texture(ImageTexture2, FragUV * vec2(0.1)).xyz;
        }
    }
	
	FragColor = vec4(Color, 1.0);
} 