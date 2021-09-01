#include "game.h"
#include <SDL.h>
#include "shader.c"

#include <stdio.h>

static s32 GlobalWindowWidth = 1280;
static s32 GlobalWindowHeight = 720;

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
}

static void LoadShaders(game_state* GameState)
{
    GameState->ShaderBox = LoadAndCreateShaders(GameState, "assets/basic.vs", "assets/basic.fs");
}

static void UpdateCamera(camera* Camera, s32 DeltaMouseX, s32 DeltaMouseY)
{
    r32 MouseSensitivity = 1.f;
    r32 XOffset = (r32)DeltaMouseX * MouseSensitivity;
    r32 YOffset = (r32)DeltaMouseY * MouseSensitivity;
    
    Camera->Yaw   += XOffset;
    Camera->Pitch -= YOffset;
    
    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (Camera->Pitch > 89.0f)
        Camera->Pitch = 89.0f;
    if (Camera->Pitch < -89.0f)
        Camera->Pitch = -89.0f;
    
    Camera->Forward[0] = (r32)cos(DegreesToRadians(Camera->Yaw)) * (r32)cos(DegreesToRadians(Camera->Pitch));
    Camera->Forward[1] = (r32)sin(DegreesToRadians(Camera->Pitch));
    Camera->Forward[2] = (r32)sin(DegreesToRadians(Camera->Yaw)) * (r32)cos(DegreesToRadians(Camera->Pitch));
    glm_normalize(Camera->Forward);
    
    glm_cross(Camera->Forward, Camera->WorldUp, Camera->Right);
    glm_normalize(Camera->Right);
    
    glm_cross(Camera->Right, Camera->Forward, Camera->Up);
    glm_normalize(Camera->Up);
}

static void AddEntity(world* World, vec3 Position, entity_types Type)
{
    Assert(World->CurrentNumEntities < MAX_NUM_ENTITIES);
    
    entity* Entity = &World->Entities[World->CurrentNumEntities++];
    Entity->Position[0] = Position[0];
    Entity->Position[1] = Position[1];
    Entity->Position[2] = Position[2];
    Entity->Type = Type;
}

void RunGame()
{
    game_state* GameState = calloc(1, sizeof(game_state));
    if(!GameState)
    {
        printf("Failed to allocate memory\n");
        exit(1);
    }
    
    SDL_Init( SDL_INIT_VIDEO );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    SDL_Window * Window = SDL_CreateWindow("Now this is Cart Racing!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GlobalWindowWidth, GlobalWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    SDL_GLContext Context = SDL_GL_CreateContext( Window );
    SDL_SetWindowGrab(Window, 1);
    SDL_SetRelativeMouseMode(1);
    
    // Load GL extensions using glad
    if (gladLoadGL())
    {
        //glEnable(GL_DEPTH_TEST);
        //glDepthFunc(GL_LEQUAL);
        //glDisable(GL_CULL_FACE);
        //glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        printf("GLAD failed to load\n");
        exit(1);
    }
    
    LoadBox(GameState);
    LoadShaders(GameState);
    
    glm_perspective(Pi / 4, 16.0f / 9.0f, 0.01f, 1000.0f, GameState->MatrixProjection);
    
    GameState->Camera.Yaw = -90.f;
    GameState->Camera.Position[2] = 4.f;
    GameState->Camera.Forward[2] = -1.f;
    GameState->Camera.Right[0] = 1.f;
    GameState->Camera.Up[1] = 1.f;
    GameState->Camera.WorldUp[1] = 1.f;
    
    vec3 EntityPosition = {0.f, 0.f, 0.f};
    AddEntity(&GameState->World, EntityPosition, Kart);
    glm_vec3_one(EntityPosition);
    AddEntity(&GameState->World, EntityPosition, Kart);
    
    s32 IsRunning = 1;
    
    while(IsRunning)
    {
        s32 MouseDeltaX = 0;
        s32 MouseDeltaY = 0;
        
        //Input pass
        
        SDL_Event EventType;
        while( SDL_PollEvent( &EventType ) )
        {
            switch( EventType.type )
            {
                case SDL_KEYUP:
                if( EventType.key.keysym.sym == SDLK_ESCAPE )
                    IsRunning = 0;
                break;
                
                case SDL_MOUSEMOTION:
                {
                    MouseDeltaX = EventType.motion.xrel;
                    MouseDeltaY = EventType.motion.yrel;
                    break;
                }
                
                case SDL_KEYDOWN:
                {
                    switch( EventType.key.keysym.sym )
                    {
                        case SDLK_ESCAPE:
                        IsRunning = 0;
                        break;
                        
                        case SDLK_w:
                        glm_vec3_add(GameState->Camera.Position, GameState->Camera.Forward, GameState->Camera.Position);
                        break;
                        
                        case SDLK_s:
                        {
                            vec3 Backwards;
                            glm_vec3_negate_to(GameState->Camera.Forward, Backwards);
                            glm_vec3_add(GameState->Camera.Position, Backwards, GameState->Camera.Position);
                        }break;
                        case SDLK_a:
                        {
                            vec3 Left;
                            glm_vec3_negate_to(GameState->Camera.Right, Left);
                            glm_vec3_add(GameState->Camera.Position, Left, GameState->Camera.Position);
                        }break;
                        
                        case SDLK_d:
                        glm_vec3_add(GameState->Camera.Position, GameState->Camera.Right, GameState->Camera.Position);
                        break;
                    }
                }break;
            }
        }
        
        //Update pass
        
        UpdateCamera(&GameState->Camera, MouseDeltaX, MouseDeltaY);
        
        //Rendering pass
        
        glClearColor(1.0f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        vec3 Center;
        glm_vec3_add(GameState->Camera.Position, GameState->Camera.Forward, Center);
        glm_lookat(GameState->Camera.Position, Center, GameState->Camera.Up, GameState->MatrixView);
        
        glUseProgram(GameState->ShaderBox);
        glUniformMatrix4fv(1, 1, GL_FALSE, &GameState->MatrixView[0][0]);
        glUniformMatrix4fv(2, 1, GL_FALSE, &GameState->MatrixProjection[0][0]);
        
        for(u32 EntityIndex = 0; EntityIndex < GameState->World.CurrentNumEntities; ++EntityIndex)
        {
            mat4 MatrixModel;
            glm_mat4_identity(MatrixModel);
            
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
}