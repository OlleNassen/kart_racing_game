#include "terrain.h"

static void ChangeUVs(mesh *Mesh, u8 *Type, int Width, int Height, int Types)
{
	for (int i = 0; i < Width * Height; ++i)
	{
		float X = (float)(i % 2);
		float Z = (float)(i / Width);
		Mesh->UVs[i] = { X, Z };
	}
}

static void MeshHeightmap(mesh *Mesh, u8 *Data, int Width, int Height)
{
	Mesh->NumVertices = Width * Height;
	Mesh->Vertices = (vec3 *)malloc(sizeof(vec3) * Mesh->NumVertices);
	Mesh->Normals = (vec3 *)malloc(sizeof(vec3) * Mesh->NumVertices);
	Mesh->UVs = (vec2 *)malloc(sizeof(vec2) * Mesh->NumVertices);
	
	Mesh->NumIndices = (Width - 1) * (Height - 1) * 6;
	Mesh->Indices = (unsigned int *)malloc(sizeof(unsigned int) * Mesh->NumIndices);
	
	float Scale = 0.1f;
	
	for (int i = 0; i < Width * Height; ++i)
	{
		float X = (float)(i % Width);
		float Z = (float)(i / Width);
		float Y = (float)Data[i] * Scale;
		Mesh->Vertices[i] = { X, Y, Z };
		
		float UVX = (float)(i % 2);
		Mesh->UVs[i] = { UVX, Z };
	}
    
    
	for (int Z = 0; Z < Height; ++Z)
        for (int X = 0; X < Width; ++X)
    {
        // The ? : and ifs are necessary for the border cases.
        float SX = (float)(Data[(X < Width - 1 ? X + 1 : X) + Z * Width]
                           - Data[(X > 0 ? X - 1 : X) + Z * Width]);
        
		if (X == 0 || X == Width-1)
            SX *= 2;
        
        float SZ = (float)(Data[X + (Z < Height - 1 ? Z + 1 : Z) * Width]
                           - Data[X + (Z > 0 ?  Z - 1 : Z) * Width]);
        
		if (Z == 0 || Z == Height - 1)
            SZ *= 2;
        
        vec3 normal;
		normal.x = -SX * Scale;
		normal.y = 2.0f * Scale;
		normal.z = SZ * Scale;
		
        Mesh->Normals[X + Z * Width] = normalize(normal);
    }
    
	int Index = 0;
	for (int Z = 0; Z < (Height - 1); ++Z)
        for (int X = 0; X < (Width - 1); ++X)
	{
		int Corners[4];
		Corners[0] = (X + 0) + (Z + 0) * Width;
		Corners[1] = (X + 0) + (Z + 1) * Width;
		Corners[2] = (X + 1) + (Z + 0) * Width;
		Corners[3] = (X + 1) + (Z + 1) * Width;
		
		Mesh->Indices[Index++] = Corners[0];
		Mesh->Indices[Index++] = Corners[1];
		Mesh->Indices[Index++] = Corners[3];
		Mesh->Indices[Index++] = Corners[3];
		Mesh->Indices[Index++] = Corners[2];
		Mesh->Indices[Index++] = Corners[0];
	}
}

static void MeshHeightmapFromImage(mesh *Mesh, const char *Path)
{
	stbi_set_flip_vertically_on_load(true);
	int Width, Height, NumComponents;
	u8 *Data = stbi_load(Path, &Width, &Height, &NumComponents, 1);
	MeshHeightmap(Mesh, Data, Width, Height);
	stbi_image_free(Data);
}

static void MeshEmpty(mesh *Mesh, int Width, int Height)
{
	u8 *Data = (u8 *)malloc(Width * Height);
	memset(Data, 0, Width * Height);
	MeshHeightmap(Mesh, Data, Width, Height);
	free(Data);
}

static void MeshUpdateBuffers(mesh *Mesh, unsigned int VAO, unsigned int VBO, unsigned int EBO)
{
    s64 NumVertices = Mesh->NumVertices;
	s64 NumVertices3 = NumVertices * sizeof(vec3);
	s64 NumVertices2 =  NumVertices * sizeof(vec2);
	
	glBindVertexArray(VAO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Mesh->NumIndices * sizeof(u32), Mesh->Indices, GL_STATIC_DRAW);
    
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, NumVertices3 * 2 +
                 NumVertices2, 0, GL_STATIC_DRAW);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, NumVertices3, Mesh->Vertices);
	glBufferSubData(GL_ARRAY_BUFFER, NumVertices3, NumVertices3, Mesh->Normals);
	glBufferSubData(GL_ARRAY_BUFFER, NumVertices3 * 2, NumVertices2, Mesh->UVs);
	
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void *)NumVertices3);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void *)(NumVertices3 * 2));
    glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
    
    
}

static float CatmullRom(float P0, float P1, float P2, float P3, float Amount)
{
    float T0 = Amount;
    float T2 = Amount * Amount;
    float T3 = Amount * Amount * Amount;
    return 0.5f *((2 * P1) + (-P0 + P2) * T0 +
                  (2*P0 - 5*P1 + 4*P2 - P3) * T2 +
                  (-P0 + 3*P1- 3*P2 + P3) * T3);
}

static vec2 CatmullRom2D(const vec2& P0, const vec2& P1, const vec2& P2, const vec2& P3, float Amount)
{
    vec2 Result;
    Result.x = CatmullRom(P0.x, P1.x, P2.x, P3.x, Amount);
    Result.y = CatmullRom(P0.y, P1.y, P2.y, P3.y, Amount);
    return Result;
}

struct curve
{
    s64 Count;
    vec2 Points[256];
    
    void Push(vec2 NewPoint)
    {
        Points[Count++] = NewPoint;
    }
    
    s64 GeneratePoints(s64 capacity, vec2* GeneratedListResult)
    {
        s64 ResultCount = 0;
        
        for(int I = 0; I < Count - 3; ++I)
        {
            vec2* P[4] = { Points + I, Points + I + 1, Points + I + 2, Points + I + 3 };
            s64 LevelOfDetail = (int)(length(*P[1] - *P[2]));
            
            for(s64  J = 0; J < LevelOfDetail; ++J)
            {
                GeneratedListResult[ResultCount++] = CatmullRom2D(*P[0], *P[1], *P[2], *P[3], (float)J / (float)LevelOfDetail);
            }
        }
        
        return ResultCount;
    }
};
