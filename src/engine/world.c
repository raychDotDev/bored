#include "world.h"
#include "entity.h"
#include "entitylist.h"
#include <raylib.h>
#include <raymath.h>

void WorldUpdate(World *self, f32 dt) {
    Entity *cleanuppool[self->entities->size];
    for (i32 i = 0; i < self->entities->size; i++)
        cleanuppool[i] = nullptr;
    for (i32 i = 0; i < self->entities->size; i++) {
        Entity *e = self->entities->items[i];
        EntityUpdate(e, self, dt);
        if (!EntityIsAlive(self->entities->items[i])) {
            cleanuppool[i] = e;
            continue;
        }
        e->vel = Vector2Add(e->vel, Vector2Scale(self->gravity, dt));
        Vector2 nextPos = Vector2Add(e->pos, Vector2Scale(e->vel, dt));
        nextPos = Vector2Clamp(nextPos, Vector2Zero(), self->size);
        if (nextPos.x < e->rad) {
            nextPos.x = e->rad;
            e->vel.x = 0.f;
            e->collision.x = -1;
        } else if (nextPos.x > self->size.x - e->rad) {
            nextPos.x = self->size.x - e->rad;
            e->vel.x = 0.f;
            e->collision.x = 1;
        } else {
            e->collision.x = 0;
        }

        if (nextPos.y < e->rad) {
            nextPos.y = e->rad;
            e->vel.y = 0.f;
            e->collision.y = -1;
        } else if (nextPos.y > self->size.y - e->rad) {
            nextPos.y = self->size.y - e->rad;
            e->vel.y = 0.f;
            e->collision.y = 1;
        } else {
            e->collision.y = 0;
        }
        e->pos = nextPos;
    }
    for (i32 i = 0; i < self->entities->size && cleanuppool[i] != nullptr;
         i++) {
        EntityListRemove(self->entities, cleanuppool[i]);
    }
    self->time += dt;
}

World *WorldCreate(Vector2 size) {
    World *self = MemAlloc(sizeof(World));
    self->size = size;
    self->entities = EntityListCreate();
    self->gravity = Vector2Zero();
    self->time = 0.f;
    return self;
}

void WorldDestroy(World *self) {
    if (!self)
        return;
    EntityListDispose(self->entities);
    MemFree(self);
}

void WorldEntityPush(World *self, Entity *value) {
    if (!self || !value)
        return;
    EntityListPush(self->entities, value);
}

Entity *WorldEntityPop(World *self) {
    if (!self)
        return nullptr;
    return EntityListPop(self->entities);
}
void WorldEntityRemove(World *self, i32 index) {
    if (!self)
        return;
    EntityListRemoveAt(self->entities, index);
}
