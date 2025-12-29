#pragma once
typedef struct _sc Screen;
#define GAME_VERSION 0.4
void GameInit();
void GameRun();
void GameSetScreen(Screen *screen);
void GameStop();
void GameCRTOn();
void GameCRTOff();
