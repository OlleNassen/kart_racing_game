#ifndef COMMON_H
#define COMMON_H

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

using glm::vec3;
using glm::mat4;
using glm::mat3;

using glm::lookAt;
using glm::normalize;
using glm::cross;
using glm::dot;

static const r32 Pi = 3.1415f;

//Couldn't find this in cglm...
inline r32 DegreesToRadians(r32 Degrees)
{
    return (Degrees * Pi) / 180.f;
}

#endif //COMMON_H
