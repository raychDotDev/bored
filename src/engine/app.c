#include "app.h"
#include "appstate.h"
#include "resman.h"
#include "types.h"
#include <raylib.h>
#include <raymath.h>

const v2i INIT_WINDOW_SIZE = (v2i){720, 480};

typedef struct _gs {
    AppState *screen;
    bool running;
    RenderTexture canvas;
    i32 windowPrevWidth;
    i32 windowPrevHeight;
    const char *localeKey;
} App;

static App __AppInstance;

void AppReloadCanvas() {
    if (IsRenderTextureValid(__AppInstance.canvas)) {
        UnloadRenderTexture(__AppInstance.canvas);
    }
    i32 ww, wh;
    ww = GetScreenWidth();
    wh = GetScreenHeight();
    __AppInstance.canvas = LoadRenderTexture(ww, wh);
}

const char *AppGetLocale() { return __AppInstance.localeKey; }
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
        __AppInstance.localeKey = available[0];
        return;
    }
    __AppInstance.localeKey = key;
}
void AppInit() {
    __AppInstance = (App){.screen = nullptr, .running = true};
    InitAudioDevice();
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(INIT_WINDOW_SIZE.x, INIT_WINDOW_SIZE.y, "Game");
	SetTargetFPS(60);
    ResManInit();
    AppSetLocale("en");
    SetWindowTitle(TextFormat(
        "%s v.%s", ResManGetLocaleString(AppGetLocale(), "title", nullptr),
        GAME_VERSION));
    __AppInstance.windowPrevWidth = INIT_WINDOW_SIZE.x;
    __AppInstance.windowPrevHeight = INIT_WINDOW_SIZE.y;
    AppReloadCanvas();
}

void AppDispose();
void AppDraw() {
    BeginTextureMode(__AppInstance.canvas);
    ClearBackground(BLACK);
    AppStateDraw(__AppInstance.screen);
    EndTextureMode();

    i32 ww, wh;
    ww = GetScreenWidth();
    wh = GetScreenHeight();
    if (ww != __AppInstance.windowPrevWidth ||
        wh != __AppInstance.windowPrevHeight) {
        AppReloadCanvas();
        __AppInstance.windowPrevWidth = ww;
        __AppInstance.windowPrevHeight = wh;
    }
    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(__AppInstance.canvas.texture,
                   (Rectangle){
                       0,
                       0,
                       __AppInstance.canvas.texture.width,
                       -__AppInstance.canvas.texture.height,
                   },
                   (Rectangle){
                       0,
                       0,
                       __AppInstance.canvas.texture.width,
                       __AppInstance.canvas.texture.height,
                   },
                   Vector2Zero(), 0.f, WHITE);
    EndDrawing();
}
void AppUpdate() { AppStateUpdate(__AppInstance.screen, GetFrameTime()); }
void AppSetState(AppState *state) {
    AppStateUnload(__AppInstance.screen);
    if (__AppInstance.screen)
        AppStateDispose(__AppInstance.screen);
    __AppInstance.screen = state;
    AppStateLoad(state);
}
void AppRun() {
    if (!__AppInstance.running) {
        return;
    }
    while (__AppInstance.running && !WindowShouldClose()) {
        AppUpdate();
        AppDraw();
    }
    AppDispose();
}
void AppStop();

void AppDispose() {
    AppSetState(nullptr);
    ResManDispose();
    UnloadRenderTexture(__AppInstance.canvas);
    CloseWindow();
    CloseAudioDevice();
}
