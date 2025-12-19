#include "engine/world.h"
#include "engine/entity.h"
#include "fixed.h"
#include <raylib.h>
#include <raymath.h>
#include <stdlib.h>

World *WorldCreate(v2 size) {
    World *self = MemAlloc(sizeof(World));
    self->entities_capacity = 16;
    self->entities_count = 0;
    self->entities = MemAlloc(sizeof(Entity *) * self->entities_capacity);
    self->size = Vector2ClampValue(size, 20, 1e30f);
    self->gravity = (v2){0, 550};
    self->inner_color = WHITE;
    self->outer_color = BLACK;
    self->contrast_color = RED;
    return self;
}
void WorldDestroy(World *self) {
    if (!self)
        return;
    for (i32 i = 0; i < self->entities_count; i++) {
        EntityDispose(self->entities[i]);
    }
    MemFree(self);
}

void WorldAddEntity(World *self, Entity *value) {
    if (!self)
        return;
    if (self->entities_count >= self->entities_capacity) {
        self->entities_capacity *= 2;
        self->entities = MemRealloc(self->entities,
                                    sizeof(Entity *) * self->entities_capacity);
    }
    self->entities[self->entities_count] = value;
    self->entities_count++;
}
i32 _w_sort(const void *_a, const void *_b) {
    Entity *a = *(Entity *const *)_a;
    Entity *b = *(Entity *const *)_b;
    i32 res = 1;
    if (!a && !b)
        res = 0;
    if (!a && b)
        res = 1;
    if (a && !b)
        res = -1;
    return res;
}
void WorldRemoveEntity(World *self, u32 at) {
    if (!self || at >= self->entities_count || !self->entities[at])
        return;
    EntityDispose(self->entities[at]);
    self->entities[at] = nullptr;
    qsort(self->entities, self->entities_count, sizeof(Entity *), _w_sort);
    self->entities_count--;
}
Entity *WorldGetEntity(World *self, u32 index) {
    if (!self || index >= self->entities_count)
        return nullptr;
    return self->entities[index];
}

void WorldUpdate(World *self) {
    if (!self)
        return;
    for (i32 i = 0; i < self->entities_count; i++) {
        EntityUpdate(self->entities[i], self);
    }
}
