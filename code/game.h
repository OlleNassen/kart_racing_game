#ifndef GAME_H
#define GAME_H
#include <stdint.h>

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

enum {false = 0, true = 1};

void GameRun();

typedef struct game_state
{
    u32 Temporary;
}game_state;

#endif //GAME_H
