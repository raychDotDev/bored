#include "entity.h"
#include "engine/resman.h"
#include <raylib.h>
#include <raymath.h>

Entity EntityEmpty(Vector2 pos, f32 rad) {
    Entity self = {};
    self.flag = 0;
    self.on_collide = nullptr;
    self.position = pos;
    self.radius = rad;
    self.velocity = Vector2Zero();
    self.gravityModifier = Vector2One();
    self.on_update = nullptr;
    self.on_draw = nullptr;
    self.collisionNormal = Vector2Zero();
    return self;
}

void EntityCollide(Entity *self, Entity *other, Vector2 normal) {
    if (!self || !other)
        return;
    if (self->on_collide) {
        self->on_collide(self, other, normal);
    }
}

void EntityUpdate(Entity *self, World *ctx, f32 dt) {
    if (!self || !ctx || !self->on_update)
        return;
    self->on_update(self, ctx, dt);
}

bool EntityIsAlive(Entity *self) {
    if (!self)
        return false;
    if (!self->is_alive)
        return true;
    return self->is_alive(self);
}

void EntityDestroy(Entity *self) {
    if (!self)
        return;
    MemFree(self);
}

void EntityDraw(Entity *self, World *ctx) {
    if (!self || !ctx)
        return;
    if (self->on_draw) {
        self->on_draw(self, ctx);
    } else {
        Texture tex;
        ResManGetResource("entity_default", RES_TEXTURE, &tex);
        Rectangle src = {0, 0, tex.width, tex.height};
        Rectangle dst = {self->position.x - self->radius, self->position.y - self->radius,
                         self->radius * 2, self->radius * 2};
        DrawTexturePro(tex, src, dst, Vector2Zero(), 0.f, self->tint);
    }
}
