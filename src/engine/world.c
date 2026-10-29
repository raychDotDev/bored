#include "world.h"
#include "entity.h"
#include "entitylist.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

void WorldHandleEntityVSEdgesCollision(World *self, Entity *entity, f32 dt) {
    entity->pos = Vector2Add(entity->pos, Vector2Scale(entity->vel, dt));
    entity->pos = Vector2Clamp(entity->pos, Vector2Zero(), self->size);
    if (entity->pos.x <= entity->rad) {
        entity->collision.x = 1;
        entity->vel.x = 0;
        entity->pos.x = entity->rad;
    }
    if (entity->pos.x >= self->size.x - entity->rad) {
        entity->collision.x = -1;
        entity->vel.x = 0;
        entity->pos.x = self->size.x - entity->rad;
    }
    if (entity->pos.y <= entity->rad) {
        entity->collision.x = 1;
        entity->vel.y = 0;
        entity->pos.y = entity->rad;
    }
    if (entity->pos.y >= self->size.y - entity->rad) {
        entity->collision.y = -1;
        entity->vel.y = 0;
        entity->pos.y = self->size.y - entity->rad;
    }
}

void WorldHandleEntityVSEntityCollision(World *self, Entity *entity,
                                        Entity *other, f32 dt) {
    if (entity == other)
        return;

    f32 radsum = entity->rad + other->rad;
    f32 overlap = radsum - Vector2Distance(entity->pos, other->pos);
    if (overlap <= 0)
        return;
    Vector2 normalE =
        Vector2Normalize(Vector2Subtract(other->pos, entity->pos));
    Vector2 normalO = Vector2Negate(normalE);

    entity->pos =
        Vector2Subtract(entity->pos, Vector2Scale(normalE, overlap / 2));
    other->pos =
        Vector2Subtract(other->pos, Vector2Scale(normalO, overlap / 2));

    Vector2 perpNormalE = Vector2Rotate(normalE, PI / 2.f);
    Vector2 perpNormalO = Vector2Rotate(normalO, PI / 2.f);
    f32 angleE = Vector2Angle(perpNormalE, Vector2Normalize(entity->vel));
    f32 angleO = Vector2Angle(perpNormalO, Vector2Normalize(other->vel));

    f32 velModE = Vector2Length(entity->vel);
    f32 velModO = Vector2Length(other->vel);

    if (Vector2DotProduct(Vector2Normalize(entity->vel), normalE) > 0 ||
        velModO == 0) {
        entity->vel = Vector2Scale(perpNormalE, cosf(angleE) * velModE);
    } else {
        entity->vel = Vector2Scale(normalO, velModE);
    }

    if (Vector2DotProduct(Vector2Normalize(other->vel), normalO) > 0 ||
        velModE == 0) {
        other->vel = Vector2Scale(perpNormalO, cosf(angleO) * velModO);
    } else {
        other->vel = Vector2Scale(normalE, velModO);
    }
    // other->vel = Vector2Scale(perpNormalO, cosf(angleO) * velModO);
}

void WorldUpdate(World *self, f32 dt) {
    for (i32 i = 0; i < self->entities->size; i++) {
        Entity *entity = self->entities->items[i];
        EntityUpdate(entity, self, dt);
        entity->vel = Vector2Add(
            entity->vel,
            Vector2Scale(
                Vector2Multiply(self->gravity, entity->gravityModifier), dt));
        WorldHandleEntityVSEdgesCollision(self, entity, dt);
        for (i32 j = 0; j < self->entities->size; j++) {
            WorldHandleEntityVSEntityCollision(self, entity,
                                               self->entities->items[j], dt);
        }
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
