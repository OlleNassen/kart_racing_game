static obb_vertices FindAllVertices(obb* Obb)
{
    obb_vertices Result = {};
    
    Result.Positions[0] = Obb->Origin + Obb->Axes[0] * Obb->HalfWidths.x + Obb->Axes[1] * Obb->HalfWidths.y + Obb->Axes[2] * Obb->HalfWidths.z;
    Result.Positions[1] = Obb->Origin - Obb->Axes[0] * Obb->HalfWidths.x + Obb->Axes[1] * Obb->HalfWidths.y + Obb->Axes[2] * Obb->HalfWidths.z;
    Result.Positions[2] = Obb->Origin + Obb->Axes[0] * Obb->HalfWidths.x - Obb->Axes[1] * Obb->HalfWidths.y + Obb->Axes[2] * Obb->HalfWidths.z;
    Result.Positions[3] = Obb->Origin + Obb->Axes[0] * Obb->HalfWidths.x + Obb->Axes[1] * Obb->HalfWidths.y - Obb->Axes[2] * Obb->HalfWidths.z;
    Result.Positions[4] = Obb->Origin - Obb->Axes[0] * Obb->HalfWidths.x - Obb->Axes[1] * Obb->HalfWidths.y - Obb->Axes[2] * Obb->HalfWidths.z;
    Result.Positions[5] = Obb->Origin + Obb->Axes[0] * Obb->HalfWidths.x - Obb->Axes[1] * Obb->HalfWidths.y - Obb->Axes[2] * Obb->HalfWidths.z;
    Result.Positions[6] = Obb->Origin - Obb->Axes[0] * Obb->HalfWidths.x + Obb->Axes[1] * Obb->HalfWidths.y - Obb->Axes[2] * Obb->HalfWidths.z;
    Result.Positions[7] = Obb->Origin - Obb->Axes[0] * Obb->HalfWidths.x - Obb->Axes[1] * Obb->HalfWidths.y + Obb->Axes[2] * Obb->HalfWidths.z;
    
    return Result;
}

static vec3 ObbFurthestPointAway(obb_vertices* ObbVertices, vec3 Direction)
{
    s32 VertexFurthestAwayIndex = 0;
    r32 VertexFurthestAwayDistance = dot(ObbVertices->Positions[0], Direction);
    for(s32 VertexIndex = 1; VertexIndex < 8; ++VertexIndex)
    {
        r32 CurrentVertexDistance = dot(ObbVertices->Positions[VertexIndex], Direction);
        if(CurrentVertexDistance > VertexFurthestAwayDistance)
        {
            VertexFurthestAwayDistance = CurrentVertexDistance;
            VertexFurthestAwayIndex = VertexIndex;
        }
    }
    return ObbVertices->Positions[VertexFurthestAwayIndex];
}

static vec3 SupportObb(obb_vertices* A, obb_vertices* B, vec3 Direction)
{
    return ObbFurthestPointAway(A, Direction) - ObbFurthestPointAway(B, -Direction);
}

static vec3 TriangleFurthestPointAway(triangle_vertices* TriangleVertices, vec3 Direction)
{
    s32 VertexFurthestAwayIndex = 0;
    r32 VertexFurthestAwayDistance = dot(TriangleVertices->Positions[0], Direction);
    for(s32 VertexIndex = 1; VertexIndex < 3; ++VertexIndex)
    {
        r32 CurrentVertexDistance = dot(TriangleVertices->Positions[VertexIndex], Direction);
        if(CurrentVertexDistance > VertexFurthestAwayDistance)
        {
            VertexFurthestAwayDistance = CurrentVertexDistance;
            VertexFurthestAwayIndex = VertexIndex;
        }
    }
    return TriangleVertices->Positions[VertexFurthestAwayIndex];
}

static vec3 SupportTriangle(obb_vertices* A, triangle_vertices* B, vec3 Direction)
{
    return ObbFurthestPointAway(A, Direction) - TriangleFurthestPointAway(B, -Direction);
}

static b32 LineCase(simplex* Simplex, vec3& Direction)
{
    vec3 A = Simplex->PointList[0];
    vec3 B = Simplex->PointList[1];
    
    vec3 AB = B - A;
    vec3 AO = vec3(0,0,0) - A;
    
    if(dot(AB, AO) > 0)
    {
        Direction = cross(cross(AB, AO), AB);
    }
    else
    {
        *Simplex = {1, A};
        Direction = AO;
    }
    
    return false;
}

static b32 TriangleCase(simplex* Simplex, vec3& Direction)
{
    vec3 A = Simplex->PointList[0];
    vec3 B = Simplex->PointList[1];
    vec3 C = Simplex->PointList[2];
    
    vec3 AB = B - A;
    vec3 AC = C - A;
    vec3 AO = vec3(0,0,0) - A;
    
    vec3 ABC = cross(AB, AC);
    
    if(dot(cross(ABC, AC), AO) > 0)
    {
        if(dot(AC, AO) > 0)
        {
            *Simplex = {2, A, C};
            Direction = cross(cross(AC, AO), AC);
        }
        else
        {
            *Simplex = {2, A, B};
            return LineCase(Simplex, Direction);
        }
    }
    else
    {
        if(dot(cross(AB, ABC), AO) > 0)
        {
            *Simplex = {2, A, B};
            return LineCase(Simplex, Direction);
        }
        else
        {
            if(dot(ABC, AO) > 0)
            {
                Direction = ABC;
            }
            else
            {
                *Simplex = {3, A, B, C};
                Direction = -ABC;
            }
        }
    }
    
    return false;
}

static b32 TetrahedronCase(simplex* Simplex, vec3& Direction)
{
    vec3 A = Simplex->PointList[0];
    vec3 B = Simplex->PointList[1];
    vec3 C = Simplex->PointList[2];
    vec3 D = Simplex->PointList[3];
    
    vec3 AB = B - A;
    vec3 AC = C - A;
    vec3 AD = D - A;
    vec3 AO = vec3(0,0,0) - A;
    
    vec3 ABC = cross(AB, AC);
    vec3 ACD = cross(AC, AD);
    vec3 ADB = cross(AD, AB);
    
    if(dot(ABC, AO) > 0)
    {
        *Simplex = {3, A, B, C};
        return TriangleCase(Simplex, Direction);
    }
    if(dot(ACD, AO) > 0)
    {
        *Simplex = {3, A, C, D};
        return TriangleCase(Simplex, Direction);
    }
    if(dot(ADB, AO) > 0)
    {
        *Simplex = {3, A, D, B};
        return TriangleCase(Simplex, Direction);
    }
    
    return true;
}

static b32 HandleSimplex(simplex* Simplex, vec3& Direction)
{
    if(Simplex->Size == 2)
        return LineCase(Simplex, Direction);
    if(Simplex->Size == 3)
        return TriangleCase(Simplex, Direction);
    if(Simplex->Size == 4)
        return TetrahedronCase(Simplex, Direction);
    
    return false;
}

static b32 GJK(entity* A, entity* B)
{
    obb_vertices Shape0Vertices = FindAllVertices(&A->Obb);
    obb_vertices Shape1Vertices = FindAllVertices(&B->Obb);
    
    //You can start in any direction but apparently this is popular
    vec3 Direction = normalize(A->Obb.Origin - B->Obb.Origin);
    simplex Simplex = {};
    vec3 FirstPoint = SupportObb(&Shape0Vertices, &Shape1Vertices, Direction);
    Simplex.InsertFront(FirstPoint);
    Direction = vec3(0,0,0) - FirstPoint;
    while(true)
    {
        vec3 NewPoint = SupportObb(&Shape0Vertices, &Shape1Vertices, Direction);
        if(dot(NewPoint, Direction) < 0.f)
            return false;
        Simplex.InsertFront(NewPoint);
        if(HandleSimplex(&Simplex, Direction))
        {
            return true;
        }
    }
}

static b32 GJK(entity* A, triangle_vertices* B)
{
    obb_vertices Shape0Vertices = FindAllVertices(&A->Obb);
    
    vec3 Direction = normalize(vec3(1,1,1));
    simplex Simplex = {};
    vec3 FirstPoint = SupportTriangle(&Shape0Vertices, B, Direction);
    Simplex.InsertFront(FirstPoint);
    Direction = vec3(0,0,0) - FirstPoint;
    while(true)
    {
        vec3 NewPoint = SupportTriangle(&Shape0Vertices, B, Direction);
        if(dot(NewPoint, Direction) < 0.f)
            return false;
        Simplex.InsertFront(NewPoint);
        if(HandleSimplex(&Simplex, Direction))
        {
            return true;
        }
    }
}

static void PhysicsBroadphase(world* World, mesh* TerrainMesh)
{
    for(u32 EntityIndex = 0; EntityIndex < World->CurrentNumEntities; ++EntityIndex)
    {
        entity* Entity0 = &World->Entities[EntityIndex];
        for(u32 ComparedIndex = EntityIndex + 1; ComparedIndex < World->CurrentNumEntities; ++ComparedIndex)
        {
            entity* Entity1 = &World->Entities[ComparedIndex];
            
            if(GJK(Entity0, Entity1))
            {
                (*World->CollisionMap)[Entity0] = Entity1;
                (*World->CollisionMap)[Entity1] = Entity0;
            }
        }
    }
    
    //Test against heightmap
    for(u32 EntityIndex = 0; EntityIndex < World->CurrentNumEntities; ++EntityIndex)
    {
        entity* Entity = &World->Entities[EntityIndex];
        
        vec3 MinDirection = normalize(vec3(-1, -1, -1));
        vec3 MaxDirection = normalize(vec3(1,1,1));
        
        obb_vertices EntityVertices = FindAllVertices(&Entity->Obb);
        
        vec3 EntityMinPosition = ObbFurthestPointAway(&EntityVertices, MinDirection);
        vec3 EntityMaxPosition = ObbFurthestPointAway(&EntityVertices, MaxDirection);
        
        for(s32 X = (s32)EntityMinPosition.x; X < (s32)EntityMaxPosition.x; ++X)
        {
            for(s32 Z = (s32)EntityMinPosition.z; Z < (s32)EntityMaxPosition.z; ++Z)
            {
                s32 Width = 256;
                
                s32 Corners[4];
                Corners[0] = (X + 0) + (Z + 0) * Width;
                Corners[1] = (X + 0) + (Z + 1) * Width;
                Corners[2] = (X + 1) + (Z + 0) * Width;
                Corners[3] = (X + 1) + (Z + 1) * Width;
                
                vec3 A = TerrainMesh->Vertices[Corners[0]];
                vec3 B = TerrainMesh->Vertices[Corners[1]];
                vec3 C = TerrainMesh->Vertices[Corners[2]];
                vec3 D = TerrainMesh->Vertices[Corners[3]];
                
                triangle_vertices TriangleA = {A, B, D};
                triangle_vertices TriangleB = {D, C, A};
                
                if(GJK(Entity, &TriangleA))
                {
                    AddThrowawayEntity(World, &TriangleA, Triangle);
                    (*World->CollisionMap)[Entity] = &World->TriangleThrowaways[World->ThrowAwayNumEntities];
                }
                if(GJK(Entity, &TriangleB))
                {
                    AddThrowawayEntity(World, &TriangleB, Triangle);
                    (*World->CollisionMap)[Entity] = &World->TriangleThrowaways[World->ThrowAwayNumEntities];
                }
            }
        }
    }
}

static void ResolvePhysics(world* World)
{
    for(auto& Entity : *World->CollisionMap)
    {
        Entity.first->Position.y += 10.f;
        Entity.first->Obb.Origin.y += 10.f;
    }
    
    World->ThrowAwayNumEntities = 0;
    *World->CollisionMap = {};
}