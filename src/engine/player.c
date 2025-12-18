#include "engine/player.h"
#include "engine/entity.h"
#include <math.h>
#include <raymath.h>
#include <stdio.h>

void _ep_on_collide_entity(Entity *s, Entity *other);
void _ep_on_update(Entity *s, World *ctx);
void _ep_on_collide_wall(Entity *s, v2 normal);
Entity *PlayerNew(v2 pos, Color tint) {
    Player *self = MemAlloc(sizeof(Player));
    self->base = EntityNew();
    self->base.pos = pos;
    self->base.rad = 2.f;
    self->base.spd = 80.f;
    self->base.friction = 0.f;
    self->base.affected_by_gravity = true;
    self->base.collides_w_entity = false;
    self->base.on_collide_entity = _ep_on_collide_entity;
    self->base.on_collide_wall = _ep_on_collide_wall;
    self->base.on_update = _ep_on_update;
    self->alive = true;
    self->jump_buf = false;
    self->jump_timer = GetTime();
    self->jump_time = 0.08f;
    self->onground = false;
    self->jumps = 1;
    self->jumps_left = 1;
    self->jump_force = -105.f;
    self->stomp_force = 170.f;
    self->dash_force = 100.f;
    self->dashes = 3;
    self->dashes_left = 3;
    self->dashing = false;
    self->dash_duration = 0.20f;
    self->dash_timer = GetTime();
	self->base.tint = tint;
    return (Entity *)self;
}
void _ep_on_collide_wall(Entity *s, v2 normal) {
    Player *self = (Player *)s;
    if (normal.y < 0) {
        self->onground = true;
        self->jumps_left = self->jumps;
        self->dashes_left = self->dashes;
    }
}
void _ep_on_collide_entity(Entity *s, Entity *other) {
    Player *self = (Player *)s;
    if (other->collides_w_entity) {
        self->alive = false;
    }
}
void _ep_reset_dash(Player *self) {
    self->dashing = false;
    self->dash_timer = GetTime();
}
void _ep_on_update(Entity *s, World *ctx) {
    Player *self = (Player *)s;
    if (IsKeyPressed(KEY_SPACE)) {
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
            _ep_reset_dash(self);
        }
    } else if (time_diff > self->jump_time) {
        self->jump_buf = false;
    }
    f32 hor = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    if (IsKeyPressed(KEY_LEFT_SHIFT) && !self->dashing &&
        self->dashes_left > 0 && hor != 0) {
        self->base.vel.x = hor * self->dash_force;
        self->dashing = true;
        self->dash_timer = GetTime();
        self->dashes_left--;
    } else if (!self->dashing) {
        self->base.vel.x = hor * self->base.spd;
    } else if (self->dashing) {
        self->base.affected_by_gravity = false;
        self->base.vel.y = 0;
    }
    if (IsKeyPressed(KEY_S)) {
        _ep_reset_dash(self);
        self->base.vel.y = self->stomp_force;
		self->base.vel.x = 0.f;
    }
    if (GetTime() - self->dash_timer > self->dash_duration) {
        _ep_reset_dash(self);
        self->base.affected_by_gravity = true;
    }
    self->onground = false;
}
