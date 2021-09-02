
static game_button ResetButton(game_button Button)
{
    game_button Result = Button;
    Result.Pressed = 0;
    Result.Released = 0;
    return Result;
}

static game_input ResetInput(game_input Input)
{
    game_input Result = Input;
    Result.Select = ResetButton(Input.Select);
    Result.Cancel = ResetButton(Input.Cancel);
    
    Result.Up = ResetButton(Input.Up);
    Result.Down = ResetButton(Input.Down);
    Result.Left = ResetButton(Input.Left);
    Result.Right = ResetButton(Input.Right);
    
    Result.Pause = ResetButton(Input.Pause);
    return Result;
}


inline game_button Pressed(game_button Button)
{
    game_button Result = Button;
    ++Result.Pressed;
    Result.Down = true;
    return Result;
}

inline game_button Released(game_button Button)
{
    game_button Result = Button;
    ++Result.Released;
    Result.Down = false;
    return Result;
}

static void HandleInput(game_state* GameState)
{
    GameState->MouseDeltaX = 0;
    GameState->MouseDeltaY = 0;
    
    for (s64 I = 0; I < ArrayCount(GameState->Players); ++I)
    {
        GameState->Players[I] = ResetInput(GameState->Players[I]);
    }
    
    SDL_Event EventType;
    while( SDL_PollEvent( &EventType ) )
    {
        switch( EventType.type )
        {
            case SDL_MOUSEMOTION:
            {
                GameState->MouseDeltaX = EventType.motion.xrel;
                GameState->MouseDeltaY = EventType.motion.yrel;
                break;
            }
            
            case SDL_KEYDOWN:
            {
                switch( EventType.key.keysym.sym )
                {
                    case SDLK_ESCAPE:
                    GameState->Players[0].Pause = Pressed(GameState->Players[0].Pause);
                    GlobalIsRunning = 0;
                    break;
                    
                    case SDLK_RETURN:
                    GameState->Players[0].Select = Pressed(GameState->Players[0].Select);
                    break;
                    
                    case SDLK_BACKSPACE:
                    GameState->Players[0].Cancel = Pressed(GameState->Players[0].Cancel);
                    break;
                    
                    case SDLK_w:
                    GameState->Players[0].Up = Pressed(GameState->Players[0].Up);
                    GameState->Camera.Position += GameState->Camera.Forward;
                    break;
                    
                    case SDLK_s:
                    {
                        GameState->Players[0].Down = Pressed(GameState->Players[0].Down);
                        
                        GameState->Camera.Position -= GameState->Camera.Forward;
                    }break;
                    case SDLK_a:
                    {
                        GameState->Players[0].Left = Pressed(GameState->Players[0].Left);
                        GameState->Camera.Position -= GameState->Camera.Right;
                    }break;
                    
                    case SDLK_d:
                    GameState->Players[0].Right = Pressed(GameState->Players[0].Right);
                    GameState->Camera.Position += GameState->Camera.Right;
                    break;
                }
            }break;
            
            case SDL_KEYUP:
            {
                switch( EventType.key.keysym.sym )
                {
                    case SDLK_ESCAPE:
                    GameState->Players[0].Pause = Released(GameState->Players[0].Pause);
                    GlobalIsRunning = 0;
                    break;
                    
                    case SDLK_RETURN:
                    GameState->Players[0].Select = Released(GameState->Players[0].Select);
                    break;
                    
                    case SDLK_BACKSPACE:
                    GameState->Players[0].Cancel = Released(GameState->Players[0].Cancel);
                    break;
                    
                    case SDLK_w:
                    GameState->Players[0].Up = Released(GameState->Players[0].Up);
                    break;
                    
                    case SDLK_s:
                    {
                        GameState->Players[0].Down = Released(GameState->Players[0].Down);
                    }break;
                    case SDLK_a:
                    {
                        GameState->Players[0].Left = Released(GameState->Players[0].Left);
                    }break;
                    
                    case SDLK_d:
                    GameState->Players[0].Right = Released(GameState->Players[0].Right);
                    break;
                }
            }break;
        }
    }
}