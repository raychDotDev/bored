#include "entitylist.h"
#include "entity.h"
#include <raylib.h>
#include <stdlib.h>

EntityList *EntityListCreate() {
    EntityList *self = MemAlloc(sizeof(EntityList));
    self->size = 0;
    self->capactiy = 1;
    self->items = MemAlloc(sizeof(Entity *) * self->capactiy);
    return self;
}

void EntityListDispose(EntityList *self) {
    if (self == nullptr)
        return;
    for (i32 i = 0; i < self->size; i++) {
        Entity *e = self->items[i];
        if (e != nullptr)
            EntityDestroy(e);
    }
    MemFree(self->items);
    MemFree(self);
}

void EntityListPush(EntityList *self, Entity *item) {
    if (self == nullptr)
        return;
    if (self->size >= self->capactiy) {
        self->capactiy *= 2;
        self->items =
            MemRealloc(self->items, sizeof(Entity *) * self->capactiy);
    }
    self->items[self->size++] = item;
}

i32 EntityListSort(const void *_a, const void *_b) {
    Entity *a = (Entity *)_a;
    Entity *b = (Entity *)_b;
    i32 i = a == nullptr ? 0 : 1;
    i32 k = b == nullptr ? 0 : 1;
    return k - i;
}

Entity *EntityListPop(EntityList *self) {
    if (self == nullptr)
        return nullptr;
    Entity *out = self->size > 0 ? self->items[self->size--] : nullptr;
    return out;
}

void EntityListRemove(EntityList *self, Entity *value) {
    if (!self || !value)
        return;
    for (i32 i = 0; i < self->size; i++) {
        if (self->items[i] == value) {
            EntityListRemoveAt(self, i);
            return;
        }
    }
}

void EntityListRemoveAt(EntityList *self, i32 index) {
    if (!self || index < 0 || index > self->size)
        return;
    EntityDestroy(self->items[index]);
    self->items[index] = nullptr;
    qsort(self->items, self->size, sizeof(Entity *), EntityListSort);
    self->size--;
}
