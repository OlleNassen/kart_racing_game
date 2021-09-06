float VerticesCubePNT[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
    
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
    
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
    
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
    0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
};


static unsigned int ImageTextureCreate(const char *Path)
{
    unsigned int Texture;
    glGenTextures(1, &Texture);
    glBindTexture(GL_TEXTURE_2D, Texture);
    
    stbi_set_flip_vertically_on_load(true);
    
    int Width, Height, NumComponents;
    
    unsigned char* Data = stbi_load(Path, &Width, &Height, &NumComponents, 4);
    if (Data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width,
                     Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Data);
        stbi_image_free(Data);
        
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }
    else
    {
        printf("Could not load texture from path: %s\n", Path);
    }
    
    return Texture;
}


static void LoadBox(game_state* GameState)
{
    glGenVertexArrays(1, &GameState->VAOBox);
    glBindVertexArray(GameState->VAOBox);
    
    glGenBuffers(1, &GameState->VBOBox);
    glBindBuffer(GL_ARRAY_BUFFER, GameState->VBOBox);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VerticesCubePNT), VerticesCubePNT, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    
    
    glGenVertexArrays(1, &GameState->TerrainVAO);
    glBindVertexArray(GameState->VAOBox);
    
    glGenBuffers(1, &GameState->TerrainVBO);
    glBindBuffer(GL_ARRAY_BUFFER, GameState->TerrainVBO);
    
    glGenBuffers(1, &GameState->TerrainEBO);
    glBindBuffer(GL_ARRAY_BUFFER, GameState->TerrainEBO);
    
    MeshHeightmapFromImage(&GameState->TerrainMesh, "assets/heightmap.png");
    MeshUpdateBuffers(&GameState->TerrainMesh, 
                      GameState->TerrainVAO, GameState->TerrainVBO, GameState->TerrainEBO);
    GameState->TerrainTexture = ImageTextureCreate("assets/ice.png");
    GameState->TerrainTexture2 = ImageTextureCreate("assets/dirt.png");
}

static void LoadShaders(game_state* GameState)
{
    GameState->ShaderBox = LoadAndCreateShaders(GameState, "assets/basic.vs", "assets/basic.fs");
    GameState->TerrainShader = LoadAndCreateShaders(GameState, "assets/terrain.vs", "assets/terrain.fs");
}

static void Render(game_state* GameState, SDL_Window* Window)
{
    glClearColor(0.75f, 0.9f, 0.8f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    GameState->MatrixView = glm::lookAt(GameState->Camera.Position, GameState->Camera.Position + GameState->Camera.Forward, GameState->Camera.Up);
    
    
    {
        curve Curve = {};
        Curve.Push(vec2(0.0f, 0.0f));
        Curve.Push(vec2(20.0f, 15.0f));
        Curve.Push(vec2(50.0f, 50.0f));
        Curve.Push(vec2(90.0f, 60.0f));
        Curve.Push(vec2(110.0f, 70.0f));
        Curve.Push(vec2(115.0f, 70.0f));
        Curve.Push(vec2(120.0f, 80.0f));
        Curve.Push(vec2(90.0f, 50.0f));
        
        vec2 Points[512] = {};
        s64 Count = Curve.GeneratePoints(ArrayCount(Points), Points);
        
        glUseProgram(GameState->TerrainShader);
        
        glUniform1i(5, (GLint)Count);
        glUniform2fv(6, (GLsizei)Count, (float*)Points);
        
        glUniformMatrix4fv(1, 1, GL_FALSE, &GameState->MatrixView[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, &GameState->MatrixProjection[0][0]);
        
        mat4 MatrixModel(1.f);
        glUniformMatrix4fv(0, 1, GL_FALSE, &MatrixModel[0][0]);
        
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(3, 0);
        glBindTexture(GL_TEXTURE_2D, GameState->TerrainTexture);
        
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(4, 1);
        glBindTexture(GL_TEXTURE_2D, GameState->TerrainTexture2);
        
        glBindVertexArray(GameState->TerrainVAO);
        glDrawElements(GL_TRIANGLES, GameState->TerrainMesh.NumIndices, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, GameState->TerrainMesh.NumVertices);
    }
    
    glUseProgram(GameState->ShaderBox);
    glUniformMatrix4fv(1, 1, GL_FALSE, &GameState->MatrixView[0][0]);
    glUniformMatrix4fv(2, 1, GL_FALSE, &GameState->MatrixProjection[0][0]);
    
    for(u32 EntityIndex = 0; EntityIndex < GameState->World.CurrentNumEntities; ++EntityIndex)
    {
        mat4 MatrixModel(1.f);
        
        entity* Entity = GameState->World.Entities + EntityIndex;
        MatrixModel[3][0] = Entity->Position[0];
        MatrixModel[3][1] = Entity->Position[1];
        MatrixModel[3][2] = Entity->Position[2];
        glUniformMatrix4fv(0, 1, GL_FALSE, &MatrixModel[0][0]);
        
        switch(Entity->Type)
        {
            case Kart:
            glBindVertexArray(GameState->VAOBox);
            glDrawArrays(GL_TRIANGLES, 0, 36);
            glBindVertexArray(0);
            break;
        }
    }
    
    SDL_GL_SwapWindow(Window);
}