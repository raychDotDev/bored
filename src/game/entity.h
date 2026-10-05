#pragma once

#include "types.h"
#include <raylib.h>

typedef struct World World;
typedef struct Entity Entity;
typedef struct Entity {
    Vector2 pos, vel;
    f32 rad;
    void (*update)(Entity *self, World *ctx, f32 dt);
    bool (*isAlive)(Entity *self);
} Entity;

Entity EntityEmpty(Vector2 pos, f32 rad);
void EntityUpdate(Entity *self, World *ctx, f32 dt);
bool EntityIsAlive(Entity *self);
void EntityDestroy(Entity* self);
