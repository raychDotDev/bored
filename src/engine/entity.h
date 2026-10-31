#pragma once

#include "types.h"
#include <raylib.h>

typedef struct World World;
typedef struct Entity Entity;

typedef enum {
    COLRES_NONE = 0,
    COLRES_DYNAMIC = 1,
} EntityCollisionResolveType;

typedef struct Entity {
    u32 flag;
    EntityCollisionResolveType collisionResolveType;
    void (*on_update)(Entity *self, World *ctx, f32 dt);
    void (*on_draw)(Entity *self, World *ctx);
    bool (*is_alive)(Entity *self);
    void (*on_collide)(Entity *self, Entity *other, Vector2 normal);
    Vector2 position, velocity, collisionNormal, gravityModifier;
    f32 radius;
    Color tint;
} Entity;

Entity EntityEmpty(Vector2 pos, f32 rad);
void EntityUpdate(Entity *self, World *ctx, f32 dt);
void EntityDraw(Entity *self, World *ctx);
void EntityCollide(Entity *self, Entity *other, Vector2 normal);
bool EntityIsAlive(Entity *self);
void EntityDestroy(Entity *self);
