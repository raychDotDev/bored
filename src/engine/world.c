#include "world.h"
#include "entity.h"
#include "entitylist.h"
#include <raylib.h>

void WorldUpdate(World *self, f32 dt) {
    Entity *cleanuppool[self->entities->size];
    for (i32 i = 0; i < self->entities->size; i++)
        cleanuppool[i] = nullptr;
    for (i32 i = 0; i < self->entities->size; i++) {
        Entity *e = self->entities->items[i];
        EntityUpdate(e, self, dt);
        if (!EntityIsAlive(self->entities->items[i])) {
            cleanuppool[i] = e;
        }
    }
    for (i32 i = 0; i < self->entities->size && cleanuppool[i] != nullptr;
         i++) {
        EntityListRemove(self->entities, cleanuppool[i]);
    }
}

World *WorldCreate(Vector2 size) {
    World *self = MemAlloc(sizeof(World));
    self->size = size;
    self->entities = EntityListCreate();
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
