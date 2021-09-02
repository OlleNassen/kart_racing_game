#ifndef GAME_H
#define GAME_H
#include <glad.c>
#include <SDL.h>
#include <cglm.h>
#include "common.h"

#define MAX_NUM_ENTITIES 1444

static s32 GlobalWindowWidth = 1280;
static s32 GlobalWindowHeight = 720;
static b32 GlobalIsRunning = 1;

//Already defined in cglm?
//enum {false = 0, true = 1};

void GameRun();

typedef struct camera
{
    vec3 Position;
    vec3 Forward;
    vec3 Right;
    vec3 Up;
    vec3 WorldUp;
    // NOTE(Olle Nässén): IN DEGREES
    r32 Pitch;
    r32 Yaw;
}camera;

typedef enum entity_types{Kart, Tree, Cloud}entity_types;

typedef struct game_button
{
    s16 Pressed;
    s16 Released;
    b32 Down;
} game_button;

typedef struct game_input
{
    game_button Select;
    game_button Cancel;
    
    game_button Up;
    game_button Down;
    game_button Left;
    game_button Right;
    
    game_button Pause;
} game_input;

typedef struct entity
{
    entity_types Type;
    vec3 Position;
    vec3 Velocity;
    vec3 Acceleration;
}entity;

typedef struct world
{
    entity Entities[MAX_NUM_ENTITIES];
    u32 CurrentNumEntities;
}world;

typedef struct game_state
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
    
}game_state;

typedef struct game_options
{
    s64 ResX;
    s64 ResY;
    
} game_options;

#endif //GAME_H
