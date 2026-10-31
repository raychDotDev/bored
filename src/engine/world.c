#include "world.h"
#include "entity.h"
#include "entitylist.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

void WorldHandleEntityVSEdgesCollision(World *self, Entity *entity, f32 dt) {
    entity->position =
        Vector2Add(entity->position, Vector2Scale(entity->velocity, dt));
    entity->position =
        Vector2Clamp(entity->position, Vector2Zero(), self->size);
    if (entity->position.x <= entity->radius) {
        entity->collisionNormal.x = 1;
        entity->velocity.x = 0;
        entity->position.x = entity->radius;
    }
    if (entity->position.x >= self->size.x - entity->radius) {
        entity->collisionNormal.x = -1;
        entity->velocity.x = 0;
        entity->position.x = self->size.x - entity->radius;
    }
    if (entity->position.y <= entity->radius) {
        entity->collisionNormal.x = 1;
        entity->velocity.y = 0;
        entity->position.y = entity->radius;
    }
    if (entity->position.y >= self->size.y - entity->radius) {
        entity->collisionNormal.y = -1;
        entity->velocity.y = 0;
        entity->position.y = self->size.y - entity->radius;
    }
}

void WorldHandleEntityVSEntityCollision(World *self, Entity *entity,
                                        Entity *other, f32 dt) {
    if (entity == other)
        return;

    const f32 radsum = entity->radius + other->radius;
    const f32 overlap =
        radsum - Vector2Distance(entity->position, other->position);
    if (overlap <= 0)
        return;
    const Vector2 normalE =
        Vector2Normalize(Vector2Subtract(other->position, entity->position));
    const Vector2 normalO = Vector2Negate(normalE);

    EntityCollide(entity, other, normalE);
    EntityCollide(other, entity, normalO);

    if (entity->collisionResolveType == COLRES_NONE ||
        other->collisionResolveType == COLRES_NONE) {
        return;
    }

    const f32 pushValue = overlap / 2;
    entity->position =
        Vector2Subtract(entity->position, Vector2Scale(normalE, pushValue));
    other->position =
        Vector2Subtract(other->position, Vector2Scale(normalO, pushValue));
    const f32 rightAngle = PI / 2.f;
    const Vector2 perpNormalE = Vector2Rotate(normalE, rightAngle);
    const Vector2 perpNormalO = Vector2Rotate(normalO, rightAngle);
    const f32 angleE =
        Vector2Angle(perpNormalE, Vector2Normalize(entity->velocity));
    const f32 angleO =
        Vector2Angle(perpNormalO, Vector2Normalize(other->velocity));

    const f32 velModE = Vector2Length(entity->velocity);
    const f32 velModO = Vector2Length(other->velocity);

    // f32 weightModifierE = (other->weight / entity->weight);
    // f32 weightModifierO = (entity->weight / other->weight);

    if (Vector2DotProduct(Vector2Normalize(entity->velocity), normalE) > 0 ||
        velModO == 0) {
        entity->velocity = Vector2Scale(perpNormalE, cosf(angleE) * velModE);
    } else {
        entity->velocity = Vector2Scale(normalO, velModE);
    }
    if (Vector2DotProduct(Vector2Normalize(other->velocity), normalO) > 0 ||
        velModE == 0) {
        other->velocity = Vector2Scale(perpNormalO, cosf(angleO) * velModO);
    } else {
        other->velocity = Vector2Scale(normalE, velModO);
    }
}

void WorldUpdate(World *self, f32 dt) {
    for (i32 i = 0; i < self->entities->size; i++) {
        Entity *entity = self->entities->items[i];
        EntityUpdate(entity, self, dt);
        entity->velocity = Vector2Add(
            entity->velocity,
            Vector2Scale(
                Vector2Multiply(self->gravity, entity->gravityModifier), dt));
        for (i32 j = 0; j < self->entities->size; j++) {
            WorldHandleEntityVSEntityCollision(self, entity,
                                               self->entities->items[j], dt);
        }
        WorldHandleEntityVSEdgesCollision(self, entity, dt);
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
