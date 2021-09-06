#ifndef PHYSICS_H
#define PHYSICS_H
#include "common.h"

typedef struct obb_vertices
{
    vec3 Positions[8];
}obb_vertices;

typedef struct obb
{
    vec3 Origin; // Position
    mat3 Axes; // Local x-, y-, and z-axes
    vec3 HalfWidths; // Positive halfwidth extents of OBB along each axis
}obb;

struct triangle_vertices
{
    vec3 Positions[3];
};

struct simplex
{
    void InsertFront(vec3 NewPoint)
    {
        PointList[3] = PointList[2];
        PointList[2] = PointList[1];
        PointList[1] = PointList[0];
        PointList[0] = NewPoint;
        if(Size < 4)
            ++Size;
    }
    
    s32 Size;
    vec3 PointList[4];
};

#endif //PHYSICS_H
