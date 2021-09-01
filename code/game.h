#ifndef GAME_H
#define GAME_H
#include <glad.c>
#include <cglm.h>
#include <stdint.h>

#define Assert(Expression) if(!(Expression)) {*(int*)0 = 0;}

#define Kilobytes(Value) ((Value) * 1024LL)
#define Megabytes(Value) (Kilobytes(Value) * 1024LL)
#define Gigabytes(Value) (Megabytes(Value) * 1024LL)
#define Terabytes(Value) (Gigabytes(Value) * 1024LL)

#define ArrayCount(Array) (sizeof(Array) / sizeof(Array[0]))

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;
typedef int64_t s64;

typedef uint32_t b32;

typedef float r32;
typedef double r64;

static const r32 Pi = 3.1415f;

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

typedef struct game_state
{
    GLuint VAOBox;
    GLuint VBOBox;
    GLuint ShaderBox;
    GLuint Padding1;
    mat4 MatrixProjection;
    mat4 MatrixView;
    camera Camera;
}game_state;

#endif //GAME_H
