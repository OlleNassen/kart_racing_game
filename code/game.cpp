#include "game.h"
#include "shader.cpp"
#include "terrain.cpp"
#include "rendering.cpp"
#include "input.cpp"
#include "parser.h"
#include "physics.cpp"

static void LoadScene(const char* Path)
{
    cgltf_options Options = {0};
    cgltf_data* Data = 0;
    cgltf_result Result = cgltf_parse_file(&Options, Path, &Data);
    
    if (Result == cgltf_result_success)
    {
        /* TODO make awesome stuff */
        
        for (s64 I = 0; I < (s64)Data->meshes_count; ++I)
        {
            cgltf_mesh* Mesh = Data->meshes + I;
            
            for (s64 J = 0; J < (s64)Mesh->primitives_count; ++J)
            {
                cgltf_primitive* Primitive = Mesh->primitives + J;
                
                for (s64 K = 0; K < (s64)Primitive->attributes_count; ++K)
                {
                    cgltf_attribute* Attribute = Primitive->attributes + K;
                    cgltf_accessor* Accessor = Attribute->data;
                    cgltf_buffer_view* BufferView = Accessor->buffer_view;
                    s64 BufferSize = BufferView->size;
                    cgltf_buffer* Buffer = BufferView->buffer;
                    
                    switch (Attribute->type)
                    {
                        case cgltf_attribute_type_position:
                        {
                            
                            
                            break;
                        }
                        case cgltf_attribute_type_normal:
                        {
                            break;
                        }
                    }
                }
                
                {
                    cgltf_accessor* Accessor = Primitive->indices;
                    cgltf_buffer_view* BufferView = Accessor->buffer_view;
                    s64 BufferSize = BufferView->size;
                    cgltf_buffer* Buffer = BufferView->buffer;
                    
                }
            }
            
            
            
            cgltf_free(Data);
        }
    }
}

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
            else if (StringEquals("FPS", &Parser.Key))
            {
                Options.FPS = ToInt64(&Parser.Value);
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
    Entity->Position = Position;
    
    Entity->Obb.Origin = Position;
    Entity->Obb.Axes = mat3(1.f);
    Entity->Obb.HalfWidths = vec3(0.5f);
    
    Entity->Type = Type;
}

static void LogicUpdate(game_state* GameState, r64 Timestep)
{
    if (GameState->Players[0].Up.Down)
    {
        GameState->Camera.Position += GameState->Camera.Forward * (r32)Timestep;
    }
    if (GameState->Players[0].Down.Down)
    {
        GameState->Camera.Position -= GameState->Camera.Forward * (r32)Timestep;
    }
    if (GameState->Players[0].Left.Down)
    {
        GameState->Camera.Position -= GameState->Camera.Right * (r32)Timestep;
    }
    if (GameState->Players[0].Right.Down)
    {
        GameState->Camera.Position += GameState->Camera.Right * (r32)Timestep;
    }
    if (GameState->Players[0].Select.Down)
    {
        GameState->World.Entities[0].Position.x += 0.1f * (r32)Timestep;
        GameState->World.Entities[0].Obb.Origin.x += 0.1f * (r32)Timestep;
    }
}

void RunGame()
{
    game_state* GameState = (game_state*)calloc(1, sizeof(game_state));
    if(!GameState)
    {
        printf("Failed to allocate memory\n");
        exit(1);
    }
    
    std::unordered_map<entity*, entity*> CollisionMap;
    GameState->World.CollisionMap = &CollisionMap;
    
    game_options Options = LoadOptions("assets/options.ini");
    
    LoadScene("assets/scene.glb");
    
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
    
    for(s32 EntityIndex = 0; EntityIndex < MAX_NUM_ENTITIES / 10; ++EntityIndex)
    {
        AddEntity(&GameState->World, vec3((r32)EntityIndex * 1.5F,40.f,25.f), Kart);
    }
    
    const s64 LogicUpdateCount = 480;
    const s64 StepCount = LogicUpdateCount / Options.FPS;
    const r64 FullStep = (r64)Options.FPS / (r64)LogicUpdateCount;
    const r64 SmallStep = (r64)(LogicUpdateCount % Options.FPS) / (r64)LogicUpdateCount;
    
    while(GlobalIsRunning)
    {
        //Input pass
        HandleInput(GameState);
        
        for (s64 I = 0; I < StepCount; ++I)
        {
            LogicUpdate(GameState, FullStep);
        }
        LogicUpdate(GameState, SmallStep);
        
        PhysicsBroadphase(&GameState->World);
        ResolvePhysics(&GameState->World);
        
        //Update stuff
        UpdateCamera(&GameState->Camera, GameState->MouseDeltaX, GameState->MouseDeltaY);
        
        //Rendering pass
        Render(GameState, Window);
    }
}
