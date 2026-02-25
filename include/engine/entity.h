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
	u64 flag;
    v2 pos, vel, fri;
    f32 rad, friction, spd;
    Color tint;
    bool collides_w_entity;
	bool affected_by_gravity;
    EntityVsWorldCollisionType evwct;
    void (*on_update)(Entity *self, World *ctx, f32 dt);
    void (*on_collide_entity)(Entity *self, Entity *other, f32 dt);
    void (*on_collide_wall)(Entity *self, v2 normal, f32 dt);
    void (*on_draw)(Entity *self, World *ctx, f32 dt);
} Entity;

Entity EntityNew();
void EntityDispose(Entity *self);

void EntityDraw(Entity *self, World *ctx, f32 dt);
void EntityUpdate(Entity *self, World *ctx, f32 dt);
void EntityCollideEntity(Entity *self, Entity *other, f32 dt);
void EntityCollideWall(Entity *self, v2 normal, f32 dt);
void EntityApplyForce(Entity *self, v2 force, f32 dt);
