#include "game.h"
#include "shader.cpp"
#include "terrain.cpp"
#include "rendering.cpp"
#include "input.cpp"
#include "parser.h"
#include "physics.cpp"

struct buffer_view
{
    s32 Buffer;
    s32 ByteLength;
    s32 ByteOffset;
};

struct buffer
{
    s32 ByteLength;
    char* Uri;
    int UriLength;
    u8* Data;
};


s64 ToInt64(const char* Str, s64 Length)
{
    s64 sign = 1;
    s64 result = 0;
	for (s64 i = 0; i < Length; ++i)
	{
		if (Str[i] != '-')
        {
            result = result * 10 + (Str[i] - '0');
        }
        else if (result == 0 && Str[i] == '-')
        {
            sign = -1;
        }
        else
        {
            break;
        }
        
	}
    result *= sign;
	return result;
}


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)strlen(s) == tok->end - tok->start &&
        strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

static scene LoadScene(const char* Path)
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
    
    char JsonString[10000] = {0};
    fread(JsonString, 1, FileSize, File);
    fclose(File);
    
    jsmn_parser Parser;
    jsmntok_t Tokens[1024];
    
    jsmn_init(&Parser);
    int Count = jsmn_parse(&Parser, JsonString, FileSize, Tokens, ArrayCount(Tokens));
    if (Count < 0)
    {
        printf("Failed to parse JSON: %d\n", Count);
    }
    
    buffer_view BufferViews[4] = {};
    int ActiveBufferView = 0;
    
    buffer Buffers[4] = {};
    int ActiveBuffer = 0;
    
    for (int I = 0; I < Count; ++I)
    {
        if (jsoneq(JsonString, Tokens + I, "bufferViews") == 0) 
        {
            jsmntok_t ArrayToken = Tokens[I + 1];
            
            int Index = I + 1; 
            while (Tokens[Index].start < ArrayToken.end)
            {
                if (jsoneq(JsonString, Tokens + Index, "buffer") == 0)
                {
                    ++Index;
                    BufferViews[ActiveBufferView].Buffer = (s32)ToInt64(JsonString + Tokens[Index].start, Tokens[Index].end - Tokens[Index].start);
                    
                }
                else if (jsoneq(JsonString, Tokens + Index, "byteLength") == 0)
                {
                    ++Index;
                    BufferViews[ActiveBufferView].ByteLength = (s32)ToInt64(JsonString + Tokens[Index].start, Tokens[Index].end - Tokens[Index].start);
                }
                else if (jsoneq(JsonString, Tokens + Index, "byteOffset") == 0)
                {
                    ++Index;
                    BufferViews[ActiveBufferView].ByteOffset = (s32)ToInt64(JsonString + Tokens[Index].start, Tokens[Index].end - Tokens[Index].start);
                    ++ActiveBufferView;
                }
                
                ++Index;
            }
            I = Index - 1;
        }
        else if (jsoneq(JsonString, Tokens + I, "buffers") == 0)
        {
            jsmntok_t ArrayToken = Tokens[I + 1];
            
            int Index = I + 1; 
            while (Tokens[Index].start < ArrayToken.end)
            {
                if (jsoneq(JsonString, Tokens + Index, "byteLength") == 0)
                {
                    ++Index;
                    Buffers[ActiveBuffer].ByteLength = (s32)ToInt64(JsonString + Tokens[Index].start, Tokens[Index].end - Tokens[Index].start);
                    
                }
                else if (jsoneq(JsonString, Tokens + Index, "uri") == 0)
                {
                    ++Index;
                    Buffers[ActiveBuffer].Uri = JsonString + Tokens[Index].start;
                    Buffers[ActiveBuffer].UriLength = Tokens[Index].end - Tokens[Index].start;
                    ++ActiveBuffer;
                }
                
                
                ++Index;
            }
            I = Index;
        }
    }
    
    scene Scene = {};
    
    for (int I = 0; I < ActiveBuffer; ++I)
    {
        buffer* Buffer = Buffers + I;
        
        char TempString[128] = {"assets/"};
        for (int J = 0; J < Buffer->UriLength; ++J)
        {
            TempString[7 + J] = Buffer->Uri[J];
        }
        
        FILE* TempFile = 0;
        TempFile = fopen(TempString, "r");
        if(!TempFile)
        {
            printf("Failed to open path: %s", Buffer->Uri);
        }
        
        Buffers->Data = (u8*)malloc(Buffer->ByteLength);
        
        fread(Buffers->Data, 1, Buffer->ByteLength, TempFile);
        fclose(TempFile);
        
        GLuint VAO;
        GLuint VBO;
        GLuint EBO;
        
        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        
        int NumIndices = BufferViews[3].ByteLength / sizeof(unsigned short);
        
        glGenBuffers(1, &EBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, BufferViews[3].ByteLength, Buffer->Data + BufferViews[3].ByteOffset, GL_STATIC_DRAW);
        
        glGenBuffers(1, &VBO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, BufferViews[3].ByteOffset, Buffer->Data, GL_STATIC_DRAW);
        
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)(s64)BufferViews[0].ByteOffset);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)(s64)BufferViews[1].ByteOffset);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(vec2), (void*)(s64)BufferViews[2].ByteOffset);
        
        Scene.VAOs[I] = VAO;
        Scene.VBOs[I] = VBO;
        Scene.EBOs[I] = EBO;
        Scene.NumIndices[I] = NumIndices;
    }
    
    
    
    
    
    
    return Scene;
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
        GameState->World.Entities[3].Position.y -= 0.1f * (r32)Timestep;
        GameState->World.Entities[3].Obb.Origin.y -= 0.1f * (r32)Timestep;
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
    
    GlobalWindowWidth = (s32)Options.ResX;
    GlobalWindowHeight = (s32)Options.ResY;
    
    SDL_Init( SDL_INIT_VIDEO );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    
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
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        //glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else
    {
        printf("GLAD failed to load\n");
        exit(1);
    }
    
    GameState->Scene = LoadScene("assets/scene.gltf");
    LoadBox(GameState);
    LoadShaders(GameState);
    
    GameState->MatrixProjection = glm::perspective(Pi / 4, 16.0f / 9.0f, 0.01f, 1000.0f);
    
    GameState->Camera.Yaw = 90.f;
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
        
        PhysicsBroadphase(&GameState->World, &GameState->TerrainMesh);
        ResolvePhysics(&GameState->World);
        
        //Update stuff
        UpdateCamera(&GameState->Camera, GameState->MouseDeltaX, GameState->MouseDeltaY);
        
        //Rendering pass
        Render(GameState, Window);
    }
}
