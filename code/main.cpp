#define JSMN_STATIC
#include "jsmn.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "game.cpp"
#include "parser.cpp"
#include "kart.cpp"

int main( int argc, char* argv[] )
{
    RunGame();
    
    return 0;
}