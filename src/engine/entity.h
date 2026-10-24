#pragma once

#include "types.h"
#include <raylib.h>

typedef enum EntityCollisionFlags {
    ENTITY_COLLISION_NONE = 0,
    ENTITY_COLLISION_PASSTHROUGH = 1,
    ENTITY_COLLISION_SOLID = 2,
    ENTITY_COLLISION_DYNAMIC = 3
} EntityCollisionFlags;

typedef struct World World;
typedef struct Entity Entity;

typedef struct Entity {
    u32 flag;
    EntityCollisionFlags collisionFlag;
    void (*update)(Entity *self, World *ctx, f32 dt);
    void (*draw)(Entity *self, World *ctx);
    bool (*isAlive)(Entity *self);
    void (*collide)(Entity *self, Entity *other, Vector2 normal);
    Vector2 pos, vel, collision, gravityModifier;
    f32 rad;
} Entity;

Entity EntityEmpty(Vector2 pos, f32 rad);
void EntityUpdate(Entity *self, World *ctx, f32 dt);
void EntityDraw(Entity *self, World *ctx);
void EntityCollide(Entity *self, Entity *other, Vector2 normal);
bool EntityIsAlive(Entity *self);
void EntityDestroy(Entity *self);
