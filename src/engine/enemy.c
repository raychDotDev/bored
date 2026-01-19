#include "engine/enemy.h"
#include "engine/entity.h"
#include "engine/resman.h"
#include "engine/world.h"
#include "fixed.h"
#include <raymath.h>

void _ee_update(Entity *s, World *ctx);
void _ee_collide_e(Entity *s, Entity *other);
void _ee_collide_w(Entity *s, v2 normal);
Entity *EnemyNew(v2 pos, Color tint, Color tint2) {
    Enemy *self = MemAlloc(sizeof(Enemy));
    self->base = EntityNew();
    self->base.flag = 1;
    self->base.pos = pos;
    self->base.tint = tint;
    self->tint2 = tint;
    self->tint3 = tint2;
    self->base.rad = 3.f;
    self->base.spd = 170.f;
    self->base.evwct = EVWCT_BOUNCE;
    self->base.affected_by_gravity = false;
    self->base.on_update = _ee_update;
    self->base.on_collide_entity = _ee_collide_e;
    self->base.on_collide_wall = _ee_collide_w;
    self->dashing = false;
    self->dash_force = 190.f;
    self->dash_timer = 0.f;
    self->dash_time = 0.1f;
    self->dash_cd = 3.f;
    self->dash_cd_timer = 0.f;
    self->dist_to_dash = 30.f;
    self->target = nullptr;
    return (Entity *)self;
}
void _ee_reset_dash(Enemy *self) {
    self->dash_timer = 0.f;
    self->dash_cd_timer = 0.f;
}
void _ee_update(Entity *s, World *ctx) {
    Enemy *self = (Enemy *)s;
    if (!self->target) {
        for (i32 i = 0; i < ctx->entities_count; i++) {
            Entity *e = ctx->entities[i];
            if (e == (Entity *)self || e->flag == self->base.flag)
                continue;
            if (e->flag == 0)
                self->target = e;
        }
    }
    f32 dash_eh = Clamp(self->dash_cd_timer / self->dash_cd, 0.f, 1.f);
    v2 dir_to_target =
        Vector2Normalize(Vector2Subtract(self->target->pos, self->base.pos));
    if (!self->dashing) {
        // f32 dist_to_target = Vector2Distance(self->base.pos, self->target->pos);
        if (self->dash_cd_timer >= self->dash_cd) {
            _ee_reset_dash(self);
            self->dashing = true;
            self->base.vel = Vector2Scale(dir_to_target, self->dash_force);
            Sound s;
            ResManGetSound("enemy_dash", &s);
            PlaySound(s);
        } else {
            self->dash_cd_timer += GetFrameTime();
            EntityApplyForce(
                (Entity *)self,
                Vector2Scale(dir_to_target,
                             self->base.spd - self->base.spd * dash_eh));
        }
    } else {
        if (self->dash_timer > self->dash_time) {
            self->dashing = false;
            _ee_reset_dash(self);
        }
        self->dash_timer += GetFrameTime();
    }
    self->base.tint = ColorLerp(self->tint3, self->tint2, dash_eh);
}
void _ee_collide_e(Entity *s, Entity *other) {
    Enemy *self = (Enemy *)s;
    self->dashing = false;
    Sound sound;
    ResManGetSound("entity_collide", &sound);
    PlaySound(sound);
    // _ee_reset_dash(self);
}
void _ee_collide_w(Entity *s, v2 normal) {
    Enemy *self = (Enemy *)s;
    self->dashing = false;
    Sound sound;
    ResManGetSound("entity_collide", &sound);
    PlaySound(sound);
    // _ee_reset_dash(self);
}
