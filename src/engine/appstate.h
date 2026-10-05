#pragma once

#include "types.h"
typedef struct _st AppState;
typedef struct _st {
    void (*on_load)(AppState *self);
    void (*on_unload)(AppState *self);
    void (*on_draw)(AppState *self);
    void (*on_update)(AppState *self, f32 dt);
} AppState;

void AppStateLoad(AppState*self);
void AppStateUnload(AppState*self);
void AppStateDraw(AppState*self);
void AppStateUpdate(AppState*self, f32 dt);

AppState AppStateBase();
void AppStateDispose(AppState*self);
