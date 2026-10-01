#include "game/states/mmst.h"
#include "engine/appstate.h"
#include "engine/app.h"
#include "engine/appstate.h"
#include <raylib.h>
#include <raymath.h>

typedef struct _mmst {
    AppState base;
} MainMenuState;

void _msc_load(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
}
void _msc_unload(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
}
void _msc_draw(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
}
void _msc_update(AppState *s) {
    MainMenuState *self = (MainMenuState *)s;
}
AppState *MainMenuState_New(bool started) {
    MainMenuState *self = MemAlloc(sizeof(MainMenuState));
	self->base = AppStateBase();
    return (AppState *)self;
}
