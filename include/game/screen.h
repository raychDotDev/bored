#pragma once

typedef struct _sc Screen;
typedef struct _sc {
    void (*on_load)(Screen *self);
    void (*on_unload)(Screen *self);
    void (*on_draw)(Screen *self);
    void (*on_update)(Screen *self);
} Screen;

void ScreenLoad(Screen*self);
void ScreenUnload(Screen*self);
void ScreenDraw(Screen*self);
void ScreenUpdate(Screen*self);

Screen ScreenNew();
void ScreenDispose(Screen*self);
