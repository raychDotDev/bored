#include "game/states/mmst.h"
#include "engine/app.h"
#include "engine/appstate.h"
#include "engine/resman.h"
#include <raylib.h>
#include <raymath.h>

typedef struct _mmst {
    AppState base;
} MainMenuState;

void _msc_load(AppState *s) { MainMenuState *self = (MainMenuState *)s; }
void _msc_unload(AppState *s) { MainMenuState *self = (MainMenuState *)s; }
void _msc_draw(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
    Font f;
    bool res = ResManGetFont("departure", &f);
    if (res)
        DrawTextEx(f, "hello, мир!", Vector2Zero(), 60, 1, WHITE);
}
void _msc_update(AppState *s) { MainMenuState *self = (MainMenuState *)s; }

AppState *MainMenuState_New(bool started) {
    MainMenuState *self = MemAlloc(sizeof(MainMenuState));
    self->base = AppStateBase();
    self->base.on_draw = _msc_draw;
    return (AppState *)self;
}
