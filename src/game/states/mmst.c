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
        self->vel.x = speed * hor;
    } else {
        self->vel.x = FILerp(self->vel.x, 0.f, 0.01f, dt);
    }

    if (IsKeyPressed(KEY_SPACE)) {
        self->vel.y = -speed * 2;
    }
}

typedef struct _mmst {
    AppState base;
    Entity *player;
    World *world;
    WorldRenderer renderer;
} MainMenuState;

void _e_other_update(Entity *self, World *ctx, f32 dt) {
    self->vel = Vector2Add(
        self->vel, Vector2Scale(Vector2Normalize(Vector2Subtract(
                                    ctx->entities->items[0]->pos, self->pos)),
                                50 * dt));
}
Texture shar;

void EntityDrawTest(Entity *self, World *ctx) {
    Rectangle src = {0, 0, shar.width, shar.height};
    Rectangle dst = {self->pos.x - self->rad, self->pos.y - self->rad,
                     self->rad * 2, self->rad * 2};
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
    self->player->update = PlayerUpdate;

    WorldEntityPush(self->world, self->player);
    self->player->draw = EntityDrawTest;
    for (i32 i = 0; i < 5; i++) {
        Entity *other = MemAlloc(sizeof(Entity));
        *other = EntityEmpty(
            (Vector2){self->player->pos.x, self->player->pos.y}, 0.5f);
        other->update = _e_other_update;
        other->draw = EntityDrawTest;
        WorldEntityPush(self->world, other);
    }
    Image sharImage = GenImageColor(512, 512, BLANK);
    ImageDrawCircle(&sharImage, sharImage.width / 2, sharImage.height / 2,
                    sharImage.width / 2, WHITE);
    shar = LoadTextureFromImage(sharImage);
    UnloadImage(sharImage);
}

void _msc_unload(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    WorldDestroy(self->world);
    UnloadTexture(shar);
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
    self->renderer.target = self->player->pos;
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
