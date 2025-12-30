#include "game/game.h"
#include "engine/resman.h"
#include "fixed.h"
#include "game/screen.h"
#include <raylib.h>

const v2i INIT_WINDOW_SIZE = (v2i){720, 480};
v2i prev_win_size = {};
typedef struct _gs {
    Screen *screen;
    bool running;
    RenderTexture2D canvas;
} GameState;

GameState self;

void GameInit() {
    self = (GameState){.screen = nullptr, .running = true};
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(INIT_WINDOW_SIZE.x, INIT_WINDOW_SIZE.y, "");
    InitAudioDevice();
    // SetMasterVolume(0.3f);
    SetTargetFPS(60);
    SetWindowTitle(TextFormat("bored v.%.1f", GAME_VERSION));
    ResManInit();
    prev_win_size = INIT_WINDOW_SIZE;
    self.canvas = LoadRenderTexture(INIT_WINDOW_SIZE.x, INIT_WINDOW_SIZE.y);
}
bool crt = false;
void GameCRTOn() { crt = true; }
void GameCRTOff() { crt = false; }
void GameDispose();
void GameDraw() {
    BeginTextureMode(self.canvas);
    ClearBackground(BLACK);
    ScreenDraw(self.screen);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    Rectangle src = (Rectangle){0, 0, self.canvas.texture.width,
                                -self.canvas.texture.height};
    Rectangle dest = (Rectangle){0, 0, GetScreenWidth(), GetScreenHeight()};
    Shader s;
    bool res = false;
    if (crt)
        res = ResManGetShader("crt", &s);
    f32 t = GetTime();
    v2 ss = {GetScreenWidth(), GetScreenHeight()};
    DrawTexturePro(self.canvas.texture, src, dest, (v2){0.f, 0.f}, 0.f, WHITE);
    if (res) {
        SetShaderValue(s, GetShaderLocation(s, "time"), &t,
                       SHADER_UNIFORM_FLOAT);
        SetShaderValue(s, GetShaderLocation(s, "resolution"), &ss,
                       SHADER_UNIFORM_VEC2);
        SetShaderValue(s, GetShaderLocation(s, "tex"), &self.canvas.texture,
                       SHADER_UNIFORM_SAMPLER2D);
        BeginShaderMode(s);
    }

    DrawTexturePro(self.canvas.texture, src, dest, (v2){0.f, 0.f}, 0.f, WHITE);

    if (res) {
        EndShaderMode();
    }
    EndDrawing();
}
void gs_reload_canvas(v2i new_size) {
    UnloadRenderTexture(self.canvas);
    self.canvas = LoadRenderTexture(new_size.x * 0.5, new_size.y * 0.5);
}
void GameUpdate() {
    v2i ss = {GetScreenWidth(), GetScreenHeight()};
    if (prev_win_size.x != ss.x || prev_win_size.y != ss.y) {
        prev_win_size = ss;
        gs_reload_canvas(ss);
    }
    if (IsKeyPressed(KEY_F5)) {
        ToggleBorderlessWindowed();
    }
    ScreenUpdate(self.screen);
}
void GameSetScreen(Screen *screen) {
    ScreenUnload(self.screen);
    if (self.screen)
        ScreenDispose(self.screen);
    self.screen = screen;
    ScreenLoad(screen);
}
void GameRun() {
    if (!self.running) {
        return;
    }
    while (self.running && !WindowShouldClose()) {
        GameUpdate();
        GameDraw();
    }
    GameDispose();
}
void GameStop();

void GameDispose() {
    GameSetScreen(nullptr);
    ResManDispose();
    UnloadRenderTexture(self.canvas);
    CloseAudioDevice();
    CloseWindow();
}
