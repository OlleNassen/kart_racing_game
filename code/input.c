static void HandleInput(game_state* GameState)
{
    GameState->MouseDeltaX = 0;
    GameState->MouseDeltaY = 0;
    
    SDL_Event EventType;
    while( SDL_PollEvent( &EventType ) )
    {
        switch( EventType.type )
        {
            case SDL_KEYUP:
            if( EventType.key.keysym.sym == SDLK_ESCAPE )
                GlobalIsRunning = 0;
            break;
            
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
                    GlobalIsRunning = 0;
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
}