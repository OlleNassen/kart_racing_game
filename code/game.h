#ifndef GAME_H
#define GAME_H
#include <glad.c>
#include <SDL.h>
#include <glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "common.h"
#include "kart.h"
#include "physics.h"
#include "terrain.h"

#define MAX_NUM_ENTITIES 1000

static s32 GlobalWindowWidth = 1280;
static s32 GlobalWindowHeight = 720;
static b32 GlobalIsRunning = 1;

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

typedef enum entity_types{Kart, Triangle, Tree, Cloud}entity_types;

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

enum physics_type {PhysObb, PhysTriangle};

typedef struct entity
{
    entity_types Type;
    physics_type BodyType;
    vec3 Position;
    vec3 Velocity;
    vec3 Acceleration;
    obb Obb;
    triangle_vertices Triangle;
    kart_type Kart;
}entity;

#include <unordered_map>
typedef struct world
{
    std::unordered_map<entity*, entity*>* CollisionMap;
    entity TriangleThrowaways[MAX_NUM_ENTITIES];
    u32 ThrowAwayNumEntities;
    entity Entities[MAX_NUM_ENTITIES];
    u32 CurrentNumEntities;
}world;


struct scene
{
    s32 NumVAOs;
    s32 NumVBOs;
    s32 NumEBOs;
    
    GLuint VAOs[16];
    GLuint VBOs[16];
    GLuint EBOs[16];
    GLuint NumIndices[16];
};


typedef struct game_state
{
    GLuint VAOBox;
    GLuint VBOBox;
    GLuint ShaderBox;
    
    GLuint TerrainVAO;
    GLuint TerrainVBO;
    GLuint TerrainEBO;
    GLuint TerrainShader;
    GLuint TerrainTexture;
    GLuint TerrainTexture2;
    
    scene Scene;
    
    mesh TerrainMesh;
    
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
    s64 FPS;
} game_options;

static void AddThrowawayEntity(world* World, triangle_vertices* Triangle, entity_types Type)
{
    Assert(World->ThrowAwayNumEntities < MAX_NUM_ENTITIES);
    
    entity* Entity = &World->TriangleThrowaways[World->ThrowAwayNumEntities++];
    Entity->Triangle = *Triangle;
    Entity->Type = Type;
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

#endif //GAME_H
