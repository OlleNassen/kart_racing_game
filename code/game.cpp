#include "game.h"
#include "shader.cpp"
#include "rendering.cpp"
#include "input.cpp"
#include "parser.h"
#include "physics.cpp"

static game_options LoadOptions(const char* Path)
{
    FILE* File = 0;
    File = fopen(Path, "r");
    if(!File)
    {
        printf("Failed to open path: %s", Path);
    }
    
    fseek (File , 0 , SEEK_END);
    long FileSize = ftell(File);
    rewind (File);
    
    char Buffer[10000] = {0};
    fread(Buffer, 1, FileSize, File);
    fclose(File);
    
    
    game_options Options = {0};
    ini_parser Parser = {0};
    Parser.At = Buffer;
    
    while (Next(&Parser))
    {
        if (StringEquals("Settings", &Parser.Section))
        {
            if (StringEquals("ResX", &Parser.Key))
            {
                Options.ResX = ToInt64(&Parser.Value);
            }
            else if (StringEquals("ResY", &Parser.Key))
            {
                Options.ResY = ToInt64(&Parser.Value);
            }
        }
        else if (StringEquals("Player1", &Parser.Section))
        {
            
        }
    }
    
    return Options;
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
    
    Camera->Forward.x = (r32)cos(DegreesToRadians(Camera->Yaw)) * (r32)cos(DegreesToRadians(Camera->Pitch));
    Camera->Forward.y = (r32)sin(DegreesToRadians(Camera->Pitch));
    Camera->Forward.z = (r32)sin(DegreesToRadians(Camera->Yaw)) * (r32)cos(DegreesToRadians(Camera->Pitch));
    Camera->Forward = normalize(Camera->Forward);
    
    Camera->Right = cross(Camera->Forward, Camera->WorldUp);
    Camera->Right = normalize(Camera->Right);
    
    Camera->Up = cross(Camera->Right, Camera->Forward);
    Camera->Up = normalize(Camera->Up);
}

static void AddEntity(world* World, vec3 Position, entity_types Type)
{
    Assert(World->CurrentNumEntities < MAX_NUM_ENTITIES);
    
    entity* Entity = &World->Entities[World->CurrentNumEntities++];
    Entity->Position[0] = Position[0];
    Entity->Position[1] = Position[1];
    Entity->Position[2] = Position[2];
    
    Entity->Obb.Origin = Position;
    Entity->Obb.Axes[0] = vec3(1.f, 0.f, 0.f);
    Entity->Obb.Axes[1] = vec3(0.f, 1.f, 0.f);
    Entity->Obb.Axes[2] = vec3(0.f, 0.f, 1.f);
    Entity->Obb.HalfWidths = vec3(0.5f);
    
    Entity->Type = Type;
}

void RunGame()
{
    game_state* GameState = (game_state*)calloc(1, sizeof(game_state));
    if(!GameState)
    {
        printf("Failed to allocate memory\n");
        exit(1);
    }
    
    game_options Options = LoadOptions("assets/options.ini");
    
    GlobalWindowWidth = (s32)Options.ResX;
    GlobalWindowHeight = (s32)Options.ResY;
    
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
    SDL_SetWindowGrab(Window, SDL_TRUE);
    SDL_SetRelativeMouseMode(SDL_TRUE);
    
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
    
    GameState->MatrixProjection = glm::perspective(Pi / 4, 16.0f / 9.0f, 0.01f, 1000.0f);
    
    GameState->Camera.Yaw = -90.f;
    GameState->Camera.Position[2] = 4.f;
    GameState->Camera.Forward[2] = -1.f;
    GameState->Camera.Right[0] = 1.f;
    GameState->Camera.Up[1] = 1.f;
    GameState->Camera.WorldUp[1] = 1.f;
    
    AddEntity(&GameState->World, vec3(0,0,0), Kart);
    AddEntity(&GameState->World, vec3(0,1,0.9f), Kart);
    
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