#pragma once
typedef struct _st AppState;
#define GAME_VERSION 0.2
void AppInit();
void AppRun();
void AppSetState(AppState *screen);
void AppStop();
