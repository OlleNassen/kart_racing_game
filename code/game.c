#include "game.h"
#include "shader.c"
#include "rendering.c"
#include "input.c"

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
    
    while(GlobalIsRunning)
    {
        //Input pass
        HandleInput(GameState);
        
        //Update stuff
        UpdateCamera(&GameState->Camera, GameState->MouseDeltaX, GameState->MouseDeltaY);
        
        //Rendering pass
        Render(GameState, Window);
    }
}