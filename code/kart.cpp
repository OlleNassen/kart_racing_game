#include "kart.h"
#include "game.h"

static kart CreateKart(u32 Model, r32 MaxVel, r32 TurnSpeed)
{
    kart Result = {};
    return Result;
}

static const kart Karts[kart_type::Count] = 
{
    CreateKart(0, 5.0f, 1.0f),
    CreateKart(0, 8.0f, 0.8f),
};

static void Break(world* World, entity* Ent)
{
    const kart* Kart = &Karts[(u32)Ent->Kart];
    
}

static void Accelerate(world* World, entity* Ent)
{
    const kart* Kart = &Karts[(u32)Ent->Kart];
    
}

static void Turn(world* World, entity* Ent)
{
    const kart* Kart = &Karts[(u32)Ent->Kart];
    
}

static void Slide(world* World, entity* Ent)
{
    const kart* Kart = &Karts[(u32)Ent->Kart];
    
}


static void GivePowerUp(world* World, entity* Ent, power_up PowerUp)
{
    
}

static void UsePowerUp(world* World, entity* Ent)
{
    
}
