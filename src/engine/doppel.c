#include "engine/doppel.h"
#include "engine/entity.h"
#include "engine/resman.h"
#include "engine/world.h"
#include "fixed.h"
#include <raylib.h>
#include <raymath.h>

void _ed_on_collide_entity(Entity *s, Entity *other);
void _ed_on_update(Entity *s, World *ctx);
void _ed_on_collide_wall(Entity *s, v2 normal);

Entity *DoppelNew(v2 pos, Color tint) {
    Doppel *self = MemAlloc(sizeof(Doppel));
    self->base = EntityNew();
    self->base.flag = 2;
    self->base.pos = pos;
    self->base.rad = 4.f;
    self->base.spd = 60.f;
    self->base.friction = 0.f;
    self->base.affected_by_gravity = true;
    self->base.collides_w_entity = true;
    self->base.on_collide_entity = _ed_on_collide_entity;
    self->base.on_collide_wall = _ed_on_collide_wall;
    self->base.on_update = _ed_on_update;
    self->jump_buf = false;
    self->jump_timer = GetTime();
    self->jump_time = 0.08f;
    self->onground = false;
    // self->base.evwct = EVWCT_BOUNCE;
    self->jumps = 2;
    self->jumps_left = 1;
    self->jump_force = -145.f;
    self->stomp_force = 180.f;
    self->dash_force = 160.f;
    self->dashes = 1;
    self->dashes_left = 1;
    self->dashing = false;
    self->dash_duration = 0.40f;
    self->dash_timer = GetTime();
    self->base.tint = tint;
    return (Entity *)self;
}
void _ed_on_collide_wall(Entity *s, v2 normal) {
    Doppel *self = (Doppel *)s;
    if (normal.y < 0) {
        self->onground = true;
        self->jumps_left = self->jumps;
        self->dashes_left = self->dashes;
    }
}
void _ed_on_collide_entity(Entity *s, Entity *other) {
    Doppel *self = (Doppel *)s;
    // if (other->collides_w_entity) {
    //     self->alive = false;
    // }
}
void _ed_reset_dash(Doppel *self) {
    self->dashing = false;
    self->dash_timer = GetTime();
}
void _ed_on_update(Entity *s, World *ctx) {
    Doppel *self = (Doppel *)s;
    if (!self->target) {
        for (i32 i = 0; i < ctx->entities_count; i++) {
            Entity *e = ctx->entities[i];
            if (e->flag == self->base.flag)
                continue;
            self->target = e;
            break;
        }
    }
    {
        f32 dash_dist = self->dash_force * self->dash_duration;
        f32 dist_to_target = Vector2Distance(self->base.pos, self->target->pos);
        f32 x_diff = self->target->pos.x - self->base.pos.x;
        f32 y_diff = self->target->pos.y - self->base.pos.y;
        self->request_mleft = false;
        self->request_mright = false;
        self->request_jump = false;
        self->request_dash = false;
        self->request_stomp = false;
        if (x_diff < 0) {
            self->request_mleft = true;
        } else if (x_diff > 0) {
            self->request_mright = true;
        }
        if (y_diff < 0 && !self->dashing) {
            self->request_jump = true;
        } else if (y_diff > self->base.rad * 4 &&
                   fabsf(x_diff) <= self->base.rad * 2) {
            self->request_stomp = true;
        } else if (dist_to_target > self->base.rad * 8 &&
                   self->dashes_left > 0 &&
                   // fabsf(y_diff) <= self->base.rad * 4 &&
                   fabsf(x_diff) >= self->base.rad * 5) {
            self->request_dash = true;
        }
    }
    if (self->request_jump) {
        self->jump_buf = true;
        self->jump_timer = GetTime();
    }
    f64 time_diff = GetTime() - self->jump_timer;
    if (self->jump_buf && (self->onground || self->jumps_left > 0) &&
        time_diff <= self->jump_time) {
        self->jump_buf = false;
        self->base.vel.y = self->jump_force;
        if (!self->onground) {
            self->jumps_left--;
        }
        if (self->dashing) {
            _ed_reset_dash(self);
        }
        // Sound s;
        // ResManGetSound("enemy_dash", &s);
        // PlaySound(s);
    } else if (time_diff > self->jump_time) {
        self->jump_buf = false;
    }
    f32 hor = self->request_mright - self->request_mleft;
    if (self->request_dash && !self->dashing && self->dashes_left > 0 &&
        hor != 0) {
        self->base.vel.x = hor * self->dash_force;
        // self->base.vel.x = 0.f;
        EntityApplyForce(s, (v2){hor * self->dash_force});
        self->dashing = true;
        self->dash_timer = GetTime();
        self->dashes_left--;
        Sound s;
        ResManGetSound("enemy_dash", &s);
        PlaySound(s);
    } else if (!self->dashing) {
        // self->base.vel.x = hor * self->base.spd;
        EntityApplyForce(s, (v2){hor * self->base.spd});
    } else if (self->dashing) {
        self->base.affected_by_gravity = false;
        self->base.vel.y = 0;
    }
    if (self->request_stomp) {
        _ed_reset_dash(self);
        self->base.vel.y = self->stomp_force;
        self->base.vel.x = 0.f;
        Sound s;
        ResManGetSound("player_stomp", &s);
        PlaySound(s);
    }
    if (GetTime() - self->dash_timer > self->dash_duration) {
        _ed_reset_dash(self);
        self->base.affected_by_gravity = true;
    }
    self->onground = false;
}
