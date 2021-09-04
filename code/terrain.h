#ifndef TERRAIN_H
#define TERRAIN_H

struct mesh
{
	int NumVertices;
	vec3 *Vertices;
	vec3 *Normals;
	vec2 *UVs;
	
	int NumIndices;
	unsigned int *Indices;
};

#endif