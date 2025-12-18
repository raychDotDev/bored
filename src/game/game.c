#include "game/game.h"
#include "fixed.h"
#include "game/screen.h"

const v2i INIT_WINDOW_SIZE = (v2i){720, 480};

typedef struct _gs {
    Screen *screen;
    bool running;
} GameState;

GameState self;

void GameInit() {
    self = (GameState){.screen = nullptr, .running = true};
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(INIT_WINDOW_SIZE.x, INIT_WINDOW_SIZE.y, "Game");
}
void GameDispose();
void GameDraw() {
    BeginDrawing();
    ClearBackground(BLACK);
    ScreenDraw(self.screen);
    EndDrawing();
}
void GameUpdate() { ScreenUpdate(self.screen); }
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
        GameDraw();
        GameUpdate();
    }
    GameDispose();
}
void GameStop();

void GameDispose() {
    GameSetScreen(nullptr);
    CloseWindow();
}
