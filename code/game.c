#include "game.h"
#include <glad.c>
#include <SDL.h>
#include <cglm.h>

#include <stdio.h>

static s32 GlobalWindowWidth = 1280;
static s32 GlobalWindowHeight = 720;

void RunGame()
{
    SDL_Init( SDL_INIT_VIDEO );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
    SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );
    
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 3 );
    SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
    
    SDL_Window * Window = SDL_CreateWindow( "Cart Racing!", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, GlobalWindowWidth, GlobalWindowHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
    SDL_GLContext Context = SDL_GL_CreateContext( Window );
    
    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
    {
        exit(1);
    }
    
    s32 IsRunning = 1;
    
    while(IsRunning)
    {
        glClear( GL_COLOR_BUFFER_BIT );
        
        SDL_Event EventType;
        while( SDL_PollEvent( &EventType ) )
        {
            switch( EventType.type )
            {
                case SDL_KEYUP:
                if( EventType.key.keysym.sym == SDLK_ESCAPE )
                    IsRunning = 0;
                break;
            }
        }
        
        SDL_GL_SwapWindow(Window);
    }
}