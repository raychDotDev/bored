#include "world.h"
#include "entity.h"
#include "entitylist.h"
#include <math.h>
#include <raylib.h>
#include <raymath.h>

void WorldUpdate(World *self, f32 dt) {
    Entity *cleanuppool[self->entities->size];
    for (i32 i = 0; i < self->entities->size; i++)
        cleanuppool[i] = nullptr;
    for (i32 i = 0; i < self->entities->size; i++) {
        Entity *e = self->entities->items[i];
        EntityUpdate(e, self, dt);
        if (!EntityIsAlive(self->entities->items[i])) {
            cleanuppool[i] = e;
            continue;
        }
        e->vel = Vector2Add(
            e->vel,
            Vector2Scale(Vector2Multiply(self->gravity, e->gravityModifier),
                         dt));
        Vector2 nextPos = Vector2Add(e->pos, Vector2Scale(e->vel, dt));
        nextPos = Vector2Clamp(nextPos, Vector2Zero(), self->size);
        if (nextPos.x < e->rad) {
            nextPos.x = e->rad;
            e->vel.x = 0.f;
            e->collision.x = -1;
        } else if (nextPos.x > self->size.x - e->rad) {
            nextPos.x = self->size.x - e->rad;
            e->vel.x = 0.f;
            e->collision.x = 1;
        } else {
            e->collision.x = 0;
        }

        if (nextPos.y < e->rad) {
            nextPos.y = e->rad;
            e->vel.y = 0.f;
            e->collision.y = -1;
        } else if (nextPos.y > self->size.y - e->rad) {
            nextPos.y = self->size.y - e->rad;
            e->vel.y = 0.f;
            e->collision.y = 1;
        } else {
            e->collision.y = 0;
        }
        e->pos = nextPos;
        for (i32 k = 0; k < self->entities->size; k++) {
            Entity *o = self->entities->items[k];
            if (e == o) {
                continue;
            }
            f32 dist = Vector2Distance(e->pos, o->pos);
            f32 radsum = e->rad + o->rad;
            if (dist > radsum) {
                continue;
            }
            if (e->collisionFlag == ENTITY_COLLISION_NONE ||
                o->collisionFlag == ENTITY_COLLISION_NONE) {
                continue;
            }

            Vector2 normalO = Vector2Normalize(Vector2Subtract(o->pos, e->pos));
            Vector2 normalE = Vector2Normalize(Vector2Subtract(e->pos, o->pos));

            // if (e->collision.x != 0) {
            //     normalE.x = 0;
            // }
            // if (e->collision.y != 0) {
            //     normalE.y = 0;
            // }
            // if (o->collision.x != 0) {
            //     normalO.x = 0;
            // }
            // if (o->collision.y != 0) {
            //     normalO.y = 0;
            // }
            if (e->collisionFlag > 1 && o->collisionFlag > 1) {
                f32 overlap = radsum - dist;

                e->pos =
                    Vector2Add(e->pos, Vector2Scale(normalE, overlap * 0.5));
                o->pos =
                    Vector2Add(o->pos, Vector2Scale(normalO, overlap * 0.5));
                // f32 forceE = Vector2Length(e->vel);
                // f32 forceO = Vector2Length(o->vel);
                f32 forceE = (Vector2Length(o->vel)) * (o->rad / e->rad);
                f32 forceO = (Vector2Length(e->vel)) * (e->rad / o->rad);
                // normalE = Vector2Normalize(normalE); normalO =
                // Vector2Normalize(normalO);

                Vector2 normalPerpE = Vector2Rotate(normalE, PI / 2.f);
                f32 angleBetweenE =
                    Vector2Angle(normalPerpE, Vector2Normalize(e->vel));

                // e->vel =Vector2Subtract()
                //     Vector2Scale(normalPerpE, cosf(angleBetweenE) * forceE);
                e->vel = Vector2Scale(normalE, forceE);

                Vector2 normalPerpO = Vector2Rotate(normalO, PI / 2.f);
                f32 angleBetweenO =
                    Vector2Angle(normalPerpO, Vector2Normalize(o->vel));

                o->vel = Vector2Scale(normalO, forceO);
                // o->vel =
                //     Vector2Scale(normalPerpO, cosf(angleBetweenO) * forceO);
            }

            EntityCollide(e, o, normalO);
            EntityCollide(o, e, normalE);
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
