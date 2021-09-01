#ifndef GAME_H
#define GAME_H
#include <glad.c>
#include <cglm.h>
#include "common.h"

#define MAX_NUM_ENTITIES 1444

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
}game_state;

#endif //GAME_H
