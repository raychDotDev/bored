#include "game/screen.h"
#include <raylib.h>

Screen ScreenNew() {
    Screen res = {
        .on_load = nullptr,
        .on_unload = nullptr,
        .on_draw = nullptr,
        .on_update = nullptr,
    };
    return res;
}
void ScreenLoad(Screen *self) {
    if (!self || !self->on_load) {
        return;
    }
    self->on_load(self);
}
void ScreenUnload(Screen *self) {
    if (!self || !self->on_unload) {
        return;
    }
    self->on_unload(self);
}
void ScreenDraw(Screen *self) {
    if (!self || !self->on_draw) {
        return;
    }
    self->on_draw(self);
}
void ScreenUpdate(Screen *self) {
    if (!self || !self->on_update) {
        return;
    }
    self->on_update(self);
}

void ScreenDispose(Screen *self) {
    if (!self)
        return;
    MemFree(self);
}
