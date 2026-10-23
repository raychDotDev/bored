#include "entity.h"
#include <raylib.h>
#include <raymath.h>

Entity EntityEmpty(Vector2 pos, f32 rad) {
    Entity self = {};
    self.pos = pos;
    self.rad = rad;
    self.vel = Vector2Zero();
    self.update = nullptr;
    self.draw = nullptr;
    self.collision = Vector2Zero();
    return self;
}

void EntityUpdate(Entity *self, World *ctx, f32 dt) {
    if (!self || !ctx || !self->update)
        return;
    self->update(self, ctx, dt);
}

bool EntityIsAlive(Entity *self) {
    if (!self)
        return false;
    if (!self->isAlive)
        return true;
    return self->isAlive(self);
}

void EntityDestroy(Entity *self) {
    if (!self)
        return;
    MemFree(self);
}

void EntityDraw(Entity *self, World *ctx) {
    if (!self || !ctx)
        return;
    if (self->draw) {
        self->draw(self, ctx);
    } else {
        DrawCircleV(self->pos, self->rad, RED);
    }
}
