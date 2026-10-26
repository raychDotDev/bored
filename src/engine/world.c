#include "world.h"
#include "entity.h"
#include "entitylist.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
typedef struct worldcollisionpair {
    i32 indexA, indexB;
} worldcollisionpair;

void WorldUpdate(World *self, f32 dt) {
    Entity *cleanuppool[self->entities->size];
    for (i32 i = 0; i < self->entities->size; i++)
        cleanuppool[i] = nullptr;
    worldcollisionpair
        collisionsChecked[self->entities->size * self->entities->size];
    i32 wcpi = 0;

    for (i32 i = 0; i < self->entities->size; i++) {
        Entity *a = self->entities->items[i];
        EntityUpdate(a, self, dt);
        if (!EntityIsAlive(self->entities->items[i])) {
            cleanuppool[i] = a;
            continue;
        }
        a->vel = Vector2Add(
            a->vel,
            Vector2Scale(Vector2Multiply(self->gravity, a->gravityModifier),
                         dt));
        Vector2 nextPos = Vector2Add(a->pos, Vector2Scale(a->vel, dt));
        nextPos = Vector2Clamp(nextPos, Vector2Zero(), self->size);
        if (nextPos.x < a->rad) {
            nextPos.x = a->rad;
            a->vel.x = 0.f;
            a->collision.x = -1;
        } else if (nextPos.x > self->size.x - a->rad) {
            nextPos.x = self->size.x - a->rad;
            a->vel.x = 0.f;
            a->collision.x = 1;
        } else {
            a->collision.x = 0;
        }

        if (nextPos.y < a->rad) {
            nextPos.y = a->rad;
            a->vel.y = 0.f;
            a->collision.y = -1;
        } else if (nextPos.y > self->size.y - a->rad) {
            nextPos.y = self->size.y - a->rad;
            a->vel.y = 0.f;
            a->collision.y = 1;
        } else {
            a->collision.y = 0;
        }
        a->pos = nextPos;
        for (i32 k = 0; k < self->entities->size; k++) {
            Entity *b = self->entities->items[k];
            if (a == b) {
                continue;
            }
            bool next = false;
            for (i32 j = 0;
                 j < sizeof(collisionsChecked) / sizeof(collisionsChecked[0]);
                 j++) {
                worldcollisionpair res = collisionsChecked[j];
                if ((res.indexA == i && res.indexB == k) ||
                    (res.indexA == k && res.indexB == i)) {
                    next = true;
                    break;
                }
            }
            if (next)
                continue;
            f32 dist = Vector2Distance(a->pos, b->pos);
            f32 radsum = a->rad + b->rad;
            if (dist > radsum) {
                continue;
            }
            if (a->collisionFlag == ENTITY_COLLISION_NONE ||
                b->collisionFlag == ENTITY_COLLISION_NONE) {
                continue;
            }

            Vector2 normalA = Vector2Normalize(Vector2Subtract(a->pos, b->pos));
            Vector2 normalB = Vector2Normalize(Vector2Subtract(b->pos, a->pos));
            if (a->collisionFlag > 1 && b->collisionFlag > 1) {

                f32 velModA = Vector2Length(a->vel);
                f32 velModB = Vector2Length(b->vel);
                // normalE = Vector2Normalize(normalE); normalO =
                // Vector2Normalize(normalO);

                // Vector2 normalPerp =
                //     Vector2Normalize(Vector2Rotate(normal, PI / 2.f));
                // f32 angleBetweenE = Vector2Angle(velNormA, velNormB);
                Vector2 accelB = Vector2Scale(normalB, velModA);

                b->vel = Vector2Add(b->vel, accelB);
                b->vel = accelB;
                if (a->collision.x != 0) {
                    b->vel.x = 0;
                }
                if (a->collision.y != 0) {
                    b->vel.y = 0;
                }
                Vector2 accelA = Vector2Scale(normalA, velModB);

                // a->vel = Vector2Add(a->vel, accelA);
                a->vel = accelA;
                if (b->collision.x != 0) {
                    a->vel.x = 0;
                }
                if (b->collision.y != 0) {
                    a->vel.y = 0;
                }
            }
            EntityCollide(a, b, normalA);

            f32 overlap = radsum - dist;
            a->pos = Vector2Add(a->pos,
                                Vector2Scale(normalA, overlap / 2.f +
                                EPSILON));
            b->pos = Vector2Add(b->pos,
                                Vector2Scale(normalB, overlap / 2.f +
                                EPSILON));
            collisionsChecked[wcpi++] = (worldcollisionpair){i, k};
        }
    }
    for (i32 i = 0; i < self->entities->size && cleanuppool[i] != nullptr;
         i++) {
        EntityListRemove(self->entities, cleanuppool[i]);
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
