#pragma once

#include "fixed.h"

#define ENTITY_MAX_SPEED 100.f
typedef enum {
    EVWCT_SLIDE = 0,
    EVWCT_BOUNCE = 1 << 1,
} EntityVsWorldCollisionType;
typedef struct _e Entity;
typedef struct _w World;
typedef struct _e {
    v2 pos, vel, fri;
    f32 rad, friction, spd;
    Color tint;
    bool collides_w_entity;
	bool affected_by_gravity;
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
void EntityApplyForce(Entity *self, v2 force);
