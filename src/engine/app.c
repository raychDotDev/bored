#include "app.h"
#include "appstate.h"
#include "resman.h"
#include "types.h"
#include <raylib.h>

const v2i INIT_WINDOW_SIZE = (v2i){720, 480};

typedef struct _gs {
    AppState *screen;
    bool running;
    const char *localeKey;
} App;

App self;

const char *AppGetLocale() { return self.localeKey; }
void AppSetLocale(const char *key) {
    if (key == nullptr)
        return;
    i32 c = 0;
    const char **available = ResManGetKeys(RES_LANG, &c);
    bool found = false;
    for (i32 i = 0; i < c; i++) {
        if (TextIsEqual(available[i], key)) {
            found = true;
            break;
        }
    }
    if (!found) {
        self.localeKey = available[0];
        return;
    }
    self.localeKey = key;
}
void AppInit() {
    self = (App){.screen = nullptr, .running = true};
    InitAudioDevice();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(INIT_WINDOW_SIZE.x, INIT_WINDOW_SIZE.y, "Game");
    ResManInit();
    AppSetLocale("en");
    SetWindowTitle(TextFormat(
        "%s v.%s", ResManGetLocaleString(AppGetLocale(), "title", nullptr),
        GAME_VERSION));
}
void AppDispose();
void AppDraw() {
    BeginDrawing();
    ClearBackground(BLACK);
    AppStateDraw(self.screen);
    EndDrawing();
}
void AppUpdate() { AppStateUpdate(self.screen, GetFrameTime()); }
void AppSetState(AppState *state) {
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
