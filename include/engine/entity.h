#pragma once

#include "fixed.h"

#define ENTITY_MAX_SPEED 100.f
typedef enum {
    EVWCT_SLIDE = 0,
    EVWCT_BOUNCE = 1 << 1,
} EntityVsWorldCollisionType;
typedef enum {
    EVECT_NONE = 0,
    EVECT_SLIDE = 1 << 1,
    EVECT_BOUNCE = 1 << 2,
} EntityVsEntityCollisionType;
typedef struct _e Entity;
typedef struct _w World;
typedef struct _e {
    v2 pos, vel;
    f32 rad;
    Color tint;
    bool collides_e;
    bool collides_w;
    EntityVsEntityCollisionType evect;
    EntityVsWorldCollisionType evwct;
    void (*on_update)(Entity *self, World *ctx);
    void (*on_collide_entity)(Entity *self, Entity *other);
    void (*on_collide_wall)(Entity *self, v2 normal);
} Entity;

Entity EntityNew();
void EntityDispose(Entity *self);

void EntityUpdate(Entity *self, World *ctx);
void EntityCollideEntity(Entity *self, Entity *other);
void EntityCollideWall(Entity *self, v2 normal);
