#pragma once

#include "types.h"
#include <raylib.h>

typedef struct World World;
typedef struct Entity Entity;

typedef struct Entity {
    u32 flag;
    void (*update)(Entity *self, World *ctx, f32 dt);
    void (*draw)(Entity *self, World *ctx);
    bool (*isAlive)(Entity *self);
    void (*collide)(Entity *self, Entity *other, Vector2 normal);
    Vector2 pos, vel, collision, gravityModifier;
    f32 rad, weight;
} Entity;

Entity EntityEmpty(Vector2 pos, f32 rad);
void EntityUpdate(Entity *self, World *ctx, f32 dt);
void EntityDraw(Entity *self, World *ctx);
void EntityCollide(Entity *self, Entity *other, Vector2 normal);
bool EntityIsAlive(Entity *self);
void EntityDestroy(Entity *self);
