#include "common.h"

struct world;

typedef struct kart
{
    r32 Speed;
    r32 Rotation;
    u32 PowerUp;
    
} kart;

typedef enum power_up
{
    PowerNone,
    
} power_up;

static void Break(struct world* World, kart* Kart);
static void Accelerate(struct world* World, kart* Kart, r32 Velocity);
static void Turn(struct world* World, kart* Kart, r32 RotationAmount);
static void Slide(struct world* World, kart* Kart);
static void GivePowerUp(struct world* World, kart* Kart, power_up PowerUp);
static void UsePowerUp(struct world* World, kart* Kart);