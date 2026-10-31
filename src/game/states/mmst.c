#include "game/states/mmst.h"
#include "engine/app.h"
#include "engine/appstate.h"
#include "engine/entity.h"
#include "engine/mathutils.h"
#include "engine/world.h"
#include "engine/worldrenderer.h"
#include <raylib.h>
#include <raymath.h>

void PlayerUpdate(Entity *self, World *ctx, f32 dt) {
    i32 hor = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    f32 speed = 20;

    if (hor != 0) {
        self->velocity.x = speed * hor;
    } else {
        self->velocity.x = FILerp(self->velocity.x, 0.f, 0.01f, dt);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        self->velocity.y = -speed * 2;
    }
}

typedef struct _mmst {
    AppState base;
    Entity *player;
    World *world;
    WorldRenderer renderer;
} MainMenuState;

void _e_other_update(Entity *self, World *ctx, f32 dt) {
    self->velocity = Vector2Add(
        self->velocity,
        Vector2Scale(Vector2Normalize(Vector2Subtract(
                         ctx->entities->items[0]->position, self->position)),
                     200 * dt));
}
Texture shar;

void EntityDrawTest(Entity *self, World *ctx) {
    Rectangle src = {0, 0, shar.width, shar.height};
    Rectangle dst = {self->position.x - self->radius,
                     self->position.y - self->radius, self->radius * 2,
                     self->radius * 2};
    DrawTexturePro(shar, src, dst, Vector2Zero(), 0.f, RED);
}

void _msc_load(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    self->world = WorldCreate((Vector2){40, 22});
    self->world->gravity = (Vector2){0.f, 90.f};
    self->renderer = WorldRendererCreate(
        (Vector2){self->world->size.x / 2, self->world->size.y / 2});
    self->renderer.step = 0.01f;
    self->player = MemAlloc(sizeof(Entity));
    *self->player = EntityEmpty(Vector2Scale(self->world->size, 0.5f), 1.f);
	self->player->tint = RED;
	// self->player->collisionResolveType = COLRES_DYNAMIC;
    self->player->on_update = PlayerUpdate;

    WorldEntityPush(self->world, self->player);
    for (i32 i = 0; i < 50; i++) {
        Entity *other = MemAlloc(sizeof(Entity));
        *other = EntityEmpty(
            (Vector2){self->player->position.x + i *0.01f, self->player->position.y - 1.f},
            0.8f);
        other->collisionResolveType = COLRES_DYNAMIC;
        // other->on_update = _e_other_update;
		other->tint = BLACK;
        // other->gravityModifier = Vector2Zero();
        WorldEntityPush(self->world, other);
    }
}

void _msc_unload(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    WorldDestroy(self->world);
}

void _msc_draw(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    WorldRendererRender(&self->renderer, self->world);
    DrawFPS(10, 10);
}
void _msc_update(AppState *s, f32 dt) {
    MainMenuState *self = (MainMenuState *)s;
    WorldUpdate(self->world, dt);
    WorldRendererUpdate(&self->renderer, self->world, dt);
    if (IsKeyPressed(KEY_EQUAL)) {
        self->renderer.zoom += 0.1;
    }
    if (IsKeyPressed(KEY_MINUS)) {
        self->renderer.zoom -= 0.1;
    }
    self->renderer.target = self->player->position;
}

AppState *MainMenuState_New(bool started) {
    MainMenuState *self = MemAlloc(sizeof(MainMenuState));
    self->base = AppStateBase();
    self->base.on_draw = _msc_draw;
    self->base.on_load = _msc_load;
    self->base.on_unload = _msc_unload;
    self->base.on_update = _msc_update;
    return (AppState *)self;
}
