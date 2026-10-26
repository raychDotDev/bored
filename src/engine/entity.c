#include "entity.h"
#include <raylib.h>
#include <raymath.h>

Entity EntityEmpty(Vector2 pos, f32 rad) {
    Entity self = {};
    self.flag = 0;
    self.collisionFlag = ENTITY_COLLISION_NONE;
    self.collide = nullptr;
    self.pos = pos;
    self.rad = rad;
    self.vel = Vector2Zero();
	self.gravityModifier = Vector2One();
    self.update = nullptr;
    self.draw = nullptr;
    self.collision = Vector2Zero();
	self.weight = 1.f;
    return self;
}

void EntityCollide(Entity *self, Entity *other, Vector2 normal) {
    if (!self || !other)
        return;
    if (self->collisionFlag == ENTITY_COLLISION_NONE ||
        other->collisionFlag == ENTITY_COLLISION_NONE) {
        return;
    }
    if (self->collide) {
        self->collide(self, other, normal);
    }
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
