#pragma once

#include "types.h"
typedef struct Entity Entity;

typedef struct _entityList {
    Entity **items;
    u64 size;
    u64 capactiy;
} EntityList;

EntityList *EntityListCreate();
void EntityListDispose(EntityList *self);

void EntityListPush(EntityList *self, Entity *item);
Entity *EntityListPop(EntityList *self);

void EntityListRemoveAt(EntityList *self, i32 index);
void EntityListRemove(EntityList *self, Entity* value);
