#include "appstate.h"
#include <raylib.h>

AppState AppStateBase() {
    AppState res = {
        .on_load = nullptr,
        .on_unload = nullptr,
        .on_draw = nullptr,
        .on_update = nullptr,
    };
    return res;
}
void AppStateLoad(AppState *self) {
    if (!self || !self->on_load) {
        return;
    }
    self->on_load(self);
}
void AppStateUnload(AppState *self) {
    if (!self || !self->on_unload) {
        return;
    }
    self->on_unload(self);
}
void AppStateDraw(AppState *self) {
    if (!self || !self->on_draw) {
        return;
    }
    self->on_draw(self);
}
void AppStateUpdate(AppState *self) {
    if (!self || !self->on_update) {
        return;
    }
    self->on_update(self);
}

void AppStateDispose(AppState *self) {
    if (!self)
        return;
    MemFree(self);
}
