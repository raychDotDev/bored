#pragma once
#include <fixed.h>
typedef struct _sc Screen;
#define GAME_VERSION 0.45
f32 GameGetFrameTime();
f32 GameGetTime();
void GameInit();
void GameRun();
void GameSetScreen(Screen *screen);
void GameStop();
void GameCRTOn();
void GameCRTOff();
