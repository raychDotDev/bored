#pragma once
typedef struct _st AppState;
#define GAME_VERSION "0.9b"
void AppInit();
void AppRun();
void AppSetState(AppState *screen);
void AppStop();
void AppSetLocale(const char* key);
const char* AppGetLocale();
