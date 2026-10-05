#include "appstate.h"
#include "types.h"
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
void AppStateUpdate(AppState *self, f32 dt) {
    if (!self || !self->on_update) {
        return;
    }
    self->on_update(self, dt);
}

void AppStateDispose(AppState *self) {
    if (!self)
        return;
    MemFree(self);
}
