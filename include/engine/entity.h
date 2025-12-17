#pragma once

#include "fixed.h"
typedef struct _e Entity;
typedef struct _e {
    v2 pos, vel;
    f32 rad;
    Color tint;
    void (*collide_entity)(Entity *self, Entity *other);
    void (*collide_wall)(Entity *self, v2 normal);
} Entity;

Entity EntityNew();

void EntityDispose(Entity*self);
