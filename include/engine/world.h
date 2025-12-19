#pragma once

#include "engine/entity.h"
#include "fixed.h"
#include <raylib.h>

typedef struct _w {
    v2 size, gravity;
    Entity **entities;
    u32 entities_count;
    u32 entities_capacity;
    Color inner_color;
    Color outer_color;
	Color contrast_color;
} World;

World *WorldCreate(v2 size);
void WorldDestroy(World *self);

void WorldAddEntity(World *self, Entity *value);
void WorldRemoveEntity(World *self, u32 at);
Entity *WorldGetEntity(World *self, u32 index);

void WorldUpdate(World*self);
