#pragma once

#include "entitylist.h"
#include "types.h"
typedef struct World {
    EntityList *entities;
    Vector2 size;
	Vector2 gravity;
	f32 time;
} World;

World *WorldCreate(Vector2 size);
void WorldDestroy(World *self);

void WorldUpdate(World *self, f32 dt);
void WorldEntityPush(World *self, Entity *value);
Entity *WorldEntityPop(World *self);
void WorldEntityRemove(World *self, i32 index);
