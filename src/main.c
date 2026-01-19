#include "engine/entity.h"
#include "engine/score_ladder.h"
#include "engine/world.h"
#include "fixed.h"
#include "game/game.h"
#include "game/screens/main_screen.h"
#include <stdio.h>

i32 main() {
    InitWindow(720, 480, "game");
    SetTargetFPS(120);
    i32 gamepad = 0;
    while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);
		EndDrawing();
        if (IsGamepadAvailable(gamepad)) {
        }
    }

    CloseWindow();
    return 0;
    GameInit();
    GameSetScreen(MainScreenNew(false));
    GameRun();
    return 0;
}
