#include "app.h"
#include "appstate.h"
#include "resman.h"
#include "types.h"
#include <raylib.h>

const v2i INIT_WINDOW_SIZE = (v2i){720, 480};

typedef struct _gs {
    AppState *screen;
    bool running;
} App;

App self;

void AppInit() {
    self = (App){.screen = nullptr, .running = true};
    InitAudioDevice();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(INIT_WINDOW_SIZE.x, INIT_WINDOW_SIZE.y, "Game");
    SetWindowTitle(TextFormat("bored v.%.1f", GAME_VERSION));
    ResManInit();
}
void AppDispose();
void AppDraw() {
    BeginDrawing();
    ClearBackground(BLACK);
    AppStateDraw(self.screen);
    EndDrawing();
}
void AppUpdate() { AppStateUpdate(self.screen); }
void AppSetState(AppState*state) {
    AppStateUnload(self.screen);
    if (self.screen)
        AppStateDispose(self.screen);
    self.screen = state;
    AppStateLoad(state);
}
void AppRun() {
    if (!self.running) {
        return;
    }
    while (self.running && !WindowShouldClose()) {
        AppUpdate();
        AppDraw();
    }
    AppDispose();
}
void AppStop();

void AppDispose() {
    AppSetState(nullptr);
    ResManDispose();
    CloseWindow();
    CloseAudioDevice();
}
