#ifndef KART_H
#define KART_H

#include "common.h"

struct world;
struct entity;

enum struct power_up : u32
{
    None,
    
};

struct kart
{
    r32 Speed;
    r32 Rotation;
    power_up PowerUp;
};

enum struct kart_type : u32
{
    Default,
    Cool,
    Count
};

static void Break(world* World, entity* Ent);
static void Accelerate(world* World, entity* Ent);
static void Turn(world* World, entity* Ent);
static void Slide(world* World, entity* Ent);
static void GivePowerUp(world* World, entity* Ent, power_up PowerUp);
static void UsePowerUp(world* World, entity* Ent);

#endif