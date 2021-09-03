#ifndef GAME_H
#define GAME_H
#include <glad.c>
#include <SDL.h>
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common.h"
#include "kart.h"

#define MAX_NUM_ENTITIES 1444

static s32 GlobalWindowWidth = 1280;
static s32 GlobalWindowHeight = 720;
static b32 GlobalIsRunning = 1;

typedef glm::vec3 vec3;
typedef glm::mat4 mat4;

using glm::lookAt;
using glm::normalize;
using glm::cross;
using glm::dot;

void GameRun();

struct camera
{
    vec3 Position;
    vec3 Forward;
    vec3 Right;
    vec3 Up;
    vec3 WorldUp;
    // NOTE(Olle Nässén): IN DEGREES
    r32 Pitch;
    r32 Yaw;
};

enum entity_types : u32
{
    Kart, Tree, Cloud
};

struct game_button
{
    s16 Pressed;
    s16 Released;
    b32 Down;
};

struct game_input
{
    game_button Select;
    game_button Cancel;
    
    game_button Up;
    game_button Down;
    game_button Left;
    game_button Right;
    
    game_button Pause;
};



struct entity
{
    entity_types Type;
    vec3 Position;
    vec3 Velocity;
    vec3 Acceleration;
    kart_type Kart;
};

struct world
{
    entity Entities[MAX_NUM_ENTITIES];
    u32 CurrentNumEntities;
};

struct game_state
{
    GLuint VAOBox;
    GLuint VBOBox;
    GLuint ShaderBox;
    GLuint Padding1;
    mat4 MatrixProjection;
    mat4 MatrixView;
    camera Camera;
    world World;
    
    s32 MouseDeltaX;
    s32 MouseDeltaY;
    
    game_input Players[4];
    
};

struct game_options
{
    s64 ResX;
    s64 ResY;
    
};

#endif //GAME_H
