#include "engine/entity.h"
#include "engine/world.h"
#include "fixed.h"
#include <raylib.h>
#include <raymath.h>

Entity EntityNew() {
    Entity self = {
        .pos = {},
        .vel = {},
        .fri = {},
        .spd = 300.f,
        .rad = 1.f,
        .friction = 2.f,
        .tint = BLACK,
        .on_collide_entity = nullptr,
        .on_collide_wall = nullptr,
        .affected_by_gravity = true,
        .collides_w_entity = true,
        .evwct = EVWCT_SLIDE,
    };
    return self;
}

void EntityDispose(Entity *self) {
    if (!self)
        return;
    MemFree(self);
}

void EntityUpdate(Entity *self, World *ctx) {
    if (!self || !ctx)
        return;
    // self->fri =
    //     Vector2Add(self->fri, Vector2Scale(ctx->gravity, GetFrameTime()));
    if (self->on_update)
        self->on_update(self, ctx);
    self->vel = Vector2Add(self->vel, Vector2Scale(self->fri, GetFrameTime()));
    // self->vel =
    //     Vector2ClampValue(self->vel, -ENTITY_MAX_SPEED, ENTITY_MAX_SPEED);
    self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, GetFrameTime()));
    for (i32 i = 0; i < ctx->entities_count; i++) {
        Entity *other = ctx->entities[i];
        if (self == other)
            continue;
        f32 dist = Vector2Distance(self->pos, other->pos);
        f32 rad_sum = self->rad + other->rad;
        f32 overlap = fabsf(dist - rad_sum);
        overlap *= 0.5f;
        if (dist < rad_sum) {
            if (other->collides_w_entity && self->collides_w_entity) {
                v2 normal_self =
                    Vector2Normalize(Vector2Subtract(self->pos, other->pos));
                v2 normal_other = Vector2Negate(normal_self);
                f32 vel_self = Vector2Length(self->vel);
                f32 vel_other = Vector2Length(other->vel);
                other->vel = Vector2Scale(normal_other, vel_other);
                other->pos = Vector2Subtract(
                    other->pos, Vector2Scale(normal_self, overlap));
                self->vel = Vector2Scale(normal_self, vel_self);
                self->pos =
                    Vector2Add(self->pos, Vector2Scale(normal_self, overlap));
            }
            EntityCollideEntity(self, other);
        }
    }
    v2 world_col_normal = {};
    world_col_normal.y = self->pos.y - self->rad < 0              ? 1
                         : self->pos.y + self->rad >= ctx->size.y ? -1
                                                                  : 0;
    world_col_normal.x = self->pos.x - self->rad < 0              ? 1
                         : self->pos.x + self->rad >= ctx->size.x ? -1
                                                                  : 0;
    if (world_col_normal.x != 0 || world_col_normal.y != 0) {
        if (self->evwct == EVWCT_BOUNCE) {
            self->vel.x = world_col_normal.x != 0 ? -self->vel.x : self->vel.x;
            self->vel.y = world_col_normal.y != 0 ? -self->vel.y : self->vel.y;
        } else {
            self->vel.x = world_col_normal.x != 0 ? 0 : self->vel.x;
            self->vel.y = world_col_normal.y != 0 ? 0 : self->vel.y;
        }
        self->pos.x = Clamp(self->pos.x, self->rad + EPSILON,
                            ctx->size.x - self->rad - EPSILON);
        self->pos.y = Clamp(self->pos.y, self->rad + EPSILON,
                            ctx->size.y - self->rad - EPSILON);
        EntityCollideWall(self, world_col_normal);
    }
    self->fri = Vector2Negate(Vector2Scale(self->vel, self->friction));
    if (self->affected_by_gravity)
        EntityApplyForce(self, ctx->gravity);
}
void EntityCollideEntity(Entity *self, Entity *other) {
    if (!self || !self->on_collide_entity || !other)
        return;
    self->on_collide_entity(self, other);
}
void EntityCollideWall(Entity *self, v2 normal) {
    if (!self || !self->on_collide_wall)
        return;
    self->on_collide_wall(self, normal);
}
void EntityApplyForce(Entity *self, v2 force) {
    if (!self)
        return;
    self->vel = Vector2Add(self->vel, Vector2Scale(force, GetFrameTime()));
}
