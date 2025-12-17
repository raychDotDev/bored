#include "engine/entity.h"
#include <raylib.h>

Entity EntityNew() {
    Entity self = {
        .pos = {},
        .vel = {},
        .rad = 1.f,
        .tint = WHITE,
        .collide_entity = nullptr,
        .collide_wall = nullptr,
    };
    return self;
}
void EntityDispose(Entity *self) {
    if (!self)
        return;
    MemFree(self);
}
