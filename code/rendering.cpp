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
}

static void LoadShaders(game_state* GameState)
{
    GameState->ShaderBox = LoadAndCreateShaders(GameState, "assets/basic.vs", "assets/basic.fs");
    GameState->TerrainShader = LoadAndCreateShaders(GameState, "assets/terrain.vs", "assets/terrain.fs");
}

static void Render(game_state* GameState, SDL_Window* Window)
{
    glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    GameState->MatrixView = glm::lookAt(GameState->Camera.Position, GameState->Camera.Position + GameState->Camera.Forward, GameState->Camera.Up);
    
    
    {
        glUseProgram(GameState->TerrainShader);
        glUniformMatrix4fv(1, 1, GL_FALSE, &GameState->MatrixView[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, &GameState->MatrixProjection[0][0]);
        
        mat4 MatrixModel(1.f);
        glUniformMatrix4fv(0, 1, GL_FALSE, &MatrixModel[0][0]);
        
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