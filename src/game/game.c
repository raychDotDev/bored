#include "game/game.h"
#include "engine/resman.h"
#include "fixed.h"
#include "game/screen.h"
#include <raylib.h>

const v2i INIT_WINDOW_SIZE = (v2i){720, 480};

typedef struct _gs {
    Screen *screen;
    bool running;
} GameState;

GameState self;

void GameInit() {
    self = (GameState){.screen = nullptr, .running = true};
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(INIT_WINDOW_SIZE.x, INIT_WINDOW_SIZE.y, "");
    InitAudioDevice();
	// SetMasterVolume(0.3f);
    SetTargetFPS(240);
    SetWindowTitle(TextFormat("bored v.%.1f", GAME_VERSION));
    ResManInit();
}
void GameDispose();
void GameDraw() {
    BeginDrawing();
    ClearBackground(BLACK);
    ScreenDraw(self.screen);
    EndDrawing();
}
void GameUpdate() {
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
    CloseAudioDevice();
    CloseWindow();
}
