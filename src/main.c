#include "engine/entity.h"
#include "engine/score_ladder.h"
#include "engine/world.h"
#include "fixed.h"
#include "game/game.h"
#include "game/screens/main_screen.h"
#include <stdio.h>

i32 main() {
    GameInit();
    GameSetScreen(MainScreenNew(false));
    GameRun();
    return 0;
}
