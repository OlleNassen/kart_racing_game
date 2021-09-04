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