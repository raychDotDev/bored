#pragma once

typedef struct _sc Screen;
typedef struct _sc {
    void (*load)(Screen *self);
    void (*unload)(Screen *self);
    void (*draw)(Screen *self);
    void (*update)(Screen *self);
} Screen;

void ScreenLoad(Screen*self);
void ScreenUnload(Screen*self);
void ScreenDraw(Screen*self);
void ScreenUpdate(Screen*self);

Screen ScreenNew();
void ScreenDispose(Screen*self);
