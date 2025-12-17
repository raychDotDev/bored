#include "game/screens/main_screen.h"
#include "fixed.h"
#include "game/game.h"
#include "game/screen.h"
#include <raylib.h>

typedef struct _msc {
    Screen base;
    f32 time;
} MainScreen;

void _msc_draw(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    DrawText(TextFormat("%f", self->time), 10, 10, 20, WHITE);
}
void _msc_update(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    self->time += GetFrameTime();
    if (IsKeyPressed(KEY_SPACE))
        GameSetScreen(MainScreenNew());
}
Screen *MainScreenNew() {
    MainScreen *self = MemAlloc(sizeof(MainScreen));
    self->base = ScreenNew();
    self->base.draw = _msc_draw;
    self->base.update = _msc_update;
    return (Screen *)self;
}
