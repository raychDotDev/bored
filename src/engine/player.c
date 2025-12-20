#include "engine/player.h"
#include "engine/dash_ability.h"
#include "engine/entity.h"
#include "engine/resman.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

void _ep_on_collide_entity(Entity *s, Entity *other);
void _ep_on_update(Entity *s, World *ctx);
void _ep_on_collide_wall(Entity *s, v2 normal);
Entity *PlayerNew(v2 pos, Color tint) {
    Player *self = MemAlloc(sizeof(Player));
    self->base = EntityNew();
    self->base.flag = 0;
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
    self->jump_force = -125.f;
    self->stomp_force = 170.f;
    self->base.tint = tint;
    self->dash_abil = DashNew(160, 3, 0.1f);
    return (Entity *)self;
}
void _ep_on_collide_wall(Entity *s, v2 normal) {
    Player *self = (Player *)s;
    if (normal.y < 0) {
        self->onground = true;
        self->jumps_left = self->jumps;
        DashRestore(&self->dash_abil);
    }
}
void _ep_on_collide_entity(Entity *s, Entity *other) {
    Player *self = (Player *)s;
    if (other->collides_w_entity) {
        self->alive = false;
    }
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
        if (self->dash_abil.dashing) {
            DashReset(&self->dash_abil);
        }
        Sound s;
        ResManGetSound("player_jump", &s);
        PlaySound(s);
    } else if (time_diff > self->jump_time) {
        self->jump_buf = false;
    }
    f32 hor = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    if (IsKeyPressed(KEY_LEFT_SHIFT)) {
        if (DashPerform(&self->dash_abil, (v2){hor}, (Entity *)self)) {
            Sound s;
            ResManGetSound("player_dash", &s);
            PlaySound(s);
        }
    } else if (!self->dash_abil.dashing) {
        self->base.vel.x = hor * self->base.spd;
        self->base.affected_by_gravity = true;
    } else if (self->dash_abil.dashing) {
        self->base.affected_by_gravity = false;
        self->base.vel.y = 0;
    }
    if (IsKeyPressed(KEY_S)) {
        DashReset(&self->dash_abil);
        self->base.vel.y = self->stomp_force;
        self->base.vel.x = 0.f;
        Sound s;
        ResManGetSound("player_stomp", &s);
        PlaySound(s);
    }
    DashUpdate(&self->dash_abil, (Entity *)self);
    self->onground = false;
}
