#include "game/states/mmst.h"
#include "engine/app.h"
#include "engine/appstate.h"
#include "engine/entity.h"
#include "engine/resman.h"
#include "engine/world.h"
#include "engine/worldrenderer.h"
#include <raylib.h>
#include <raymath.h>

typedef struct _mmst {
    AppState base;
    World *world;
    WorldRenderer renderer;
} MainMenuState;

void _msc_load(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    self->world = WorldCreate((Vector2){40, 22});
	self->world->gravity = (Vector2){0.f, 10.f};
    self->renderer = WorldRendererCreate(
        (Vector2){self->world->size.x / 2, self->world->size.y / 2});
	self->renderer.step = 0.01f;
	Entity* e = MemAlloc(sizeof(Entity));
	*e = EntityEmpty(Vector2Scale(self->world->size, 0.5f), 1.f);
	WorldEntityPush(self->world, e);
}

void _msc_unload(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    WorldDestroy(self->world);
}

void _msc_draw(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    Font f;
    WorldRendererRender(&self->renderer, self->world);
    bool res = ResManGetResource("departure", RES_FONT, &f);
    if (res)
        DrawTextEx(f,
                   ResManGetLocaleString(AppGetLocale(), "startgame", nullptr),
                   Vector2Zero(), 60, 1, WHITE);
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

    i32 x, y;
    x = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    y = IsKeyDown(KEY_S) - IsKeyDown(KEY_W);
    Vector2 dir = {x, y};
    dir = Vector2Normalize(dir);
    self->renderer.target =
        Vector2Add(self->renderer.target, Vector2Scale(dir, 100 * dt));
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
