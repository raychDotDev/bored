#include "game/screen.h"
#include <raylib.h>

Screen ScreenNew() {
    Screen res = {
        .load = nullptr,
        .unload = nullptr,
        .draw = nullptr,
        .update = nullptr,
    };
    return res;
}
void ScreenLoad(Screen *self) {
    if (!self || !self->load) {
        return;
    }
    self->load(self);
}
void ScreenUnload(Screen *self) {
    if (!self || !self->unload) {
        return;
    }
    self->unload(self);
}
void ScreenDraw(Screen *self) {
    if (!self || !self->draw) {
        return;
    }
    self->draw(self);
}
void ScreenUpdate(Screen *self) {
    if (!self || !self->update) {
        return;
    }
    self->update(self);
}

void ScreenDispose(Screen *self) {
    if (!self)
        return;
    MemFree(self);
}
