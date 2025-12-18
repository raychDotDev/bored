#include "engine/entity.h"
#include "engine/world.h"
#include "fixed.h"
#include <raylib.h>
#include <raymath.h>

Entity EntityNew() {
    Entity self = {
        .pos = {},
        .vel = {},
        .rad = 1.f,
        .tint = WHITE,
        .on_collide_entity = nullptr,
        .on_collide_wall = nullptr,
        .collides_e = true,
        .collides_w = true,
        .evect = EVECT_SLIDE,
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
    if (self->on_update)
        self->on_update(self, ctx);
    self->vel =
        Vector2ClampValue(self->vel, -ENTITY_MAX_SPEED, ENTITY_MAX_SPEED);
    self->pos = Vector2Add(self->pos, Vector2Scale(self->vel, GetFrameTime()));
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
    for (i32 i = 0; i < ctx->entities_count; i++) {
        Entity *other = ctx->entities[i];
        if (self == other)
            continue;
        f32 dist = Vector2Distance(self->pos, other->pos);
        f32 rad_sum = self->rad + other->rad;
        if (dist <= rad_sum) {
            if (other->evect != EVECT_NONE) {
                f32 overlap = rad_sum - dist + EPSILON;
                v2 normal_other =
                    Vector2Normalize(Vector2Subtract(other->pos, self->pos));
                v2 normal_self =
                    Vector2Normalize(Vector2Subtract(self->pos, other->pos));
                // other->vel = Vector2Add(other->vel, self->vel);
                f32 vel_self = Vector2Length(self->vel);
                f32 vel_other = Vector2Length(other->vel);
                if (other->evect == EVECT_BOUNCE) {
                    other->vel = Vector2Scale(normal_other, vel_other);
                }
                other->pos =
                    Vector2Add(other->pos, Vector2Scale(normal_other, overlap));
                switch (self->evect) {
                case EVECT_BOUNCE: {
                    self->vel = Vector2Scale(normal_self, vel_self);
                    self->pos = Vector2Add(self->pos,
                                           Vector2Scale(normal_self, overlap));
                } break;
                case EVECT_SLIDE: {
                    self->pos = Vector2Add(self->pos,
                                           Vector2Scale(normal_self, overlap));
                } break;
                case EVECT_NONE: {
                } break;
                }
            }
            EntityCollideEntity(self, other);
        }
    }
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
