#include "game/screens/main_screen.h"
#include "engine/entity.h"
#include "engine/player.h"
#include "engine/world.h"
#include "fixed.h"
#include "game/game.h"
#include "game/screen.h"
#include <raylib.h>
#include <raymath.h>

typedef struct _msc {
    Screen base;
    Camera2D camera;
    World *w;
    bool started;
    f32 seconds;
} MainScreen;

void _msc_update_camera(MainScreen *self, v2 targetPos) {
    v2 ss = {GetScreenWidth(), GetScreenHeight()};
    v2 sshalf = {ss.x / 2, ss.y / 2};
    self->camera.target =
        Vector2Lerp(self->camera.target, targetPos, 9.f * GetFrameTime());
    self->camera.offset = sshalf;
    self->camera.rotation = 0.f;
    self->camera.zoom = fabsf(ss.y / self->w->size.y) * 0.5f;
}
void _msc_load(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    self->w = WorldCreate((v2){150, 50});
    Entity *player =
        PlayerNew((v2){self->w->size.x * 0.5f, self->w->size.y * 0.5f},
                  self->w->outer_color);
    Entity *someone = MemAlloc(sizeof(Entity));
    *someone = EntityNew();
    someone->pos = (v2){self->w->size.x * 0.1f, self->w->size.y * 0.1f};
    someone->rad = 3.f;
    someone->tint = self->w->outer_color;
    someone->spd = 400.f;
    someone->evwct = EVWCT_BOUNCE;
    someone->affected_by_gravity = false;
    Entity *somebody = MemAlloc(sizeof(Entity));
    *somebody = EntityNew();
    somebody->pos = (v2){self->w->size.x * 0.9f, self->w->size.y * 0.1f};
    somebody->rad = 3.f;
    somebody->tint = self->w->outer_color;
    somebody->spd = 360.f;
    somebody->evwct = EVWCT_BOUNCE;
    somebody->affected_by_gravity = false;
    WorldAddEntity(self->w, player);
    WorldAddEntity(self->w, someone);
    WorldAddEntity(self->w, somebody);
    self->camera = (Camera2D){.target = player->pos};
}
void _msc_unload(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    WorldDestroy(self->w);
}
void _msc_draw(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    Player *p = (Player *)self->w->entities[0];
    DrawRectangleV((v2){}, (v2){GetScreenWidth(), GetScreenHeight()},
                   self->w->outer_color);
    BeginMode2D(self->camera);
    DrawRectangleV((v2){}, self->w->size, self->w->inner_color);
    f32 ww = GetScreenWidth();
    f32 wh = GetScreenHeight();
    f32 ts = 20;
    Color outer = self->w->outer_color;
    outer.a = 150;
    const char *secs = "%.1f";
    secs = TextFormat(secs, self->seconds);
    f32 ss = MeasureText(secs, ts);
    DrawText(secs, self->w->size.x / 2 - ss / 2, self->w->size.y / 2 - ts / 2,
             ts, outer);
    for (i32 i = 0; i < self->w->entities_count; i++) {
        Entity *e = self->w->entities[i];
        DrawCircleV(e->pos, e->rad, e->tint);
    }
    EndMode2D();
    if (!p->alive || !self->started) {
        DrawRectangleV((v2){}, (v2){GetScreenWidth(), GetScreenHeight()},
                       outer);
        f32 ts = 20;
        const char *text = "PRESS [SPACE] TO (RE)START";
        f32 text_s = MeasureText(text, ts);
        DrawText(text, ww / 2.f - text_s / 2, wh / 2, ts, self->w->inner_color);
        if (!p->alive) {
            const char *secs = "YOUR SCORE: %.1f SECONDS";
            secs = TextFormat(secs, self->seconds);
            f32 secs_s = MeasureText(text, 20);
            DrawText(secs, ww / 2.f - secs_s / 2, wh / 2 - ts, ts,
                     self->w->inner_color);
        }
    }
}
void _msc_update(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    Entity *p = self->w->entities[0];
    _msc_update_camera(self, p->pos);
    if (!((Player *)p)->alive) {
        if (IsKeyPressed(KEY_SPACE)) {
            GameSetScreen(MainScreenNew(true));
        }
    } else if (((Player *)p)->alive && self->started) {
        for (i32 i = 0; i < self->w->entities_count; i++) {
            Entity *e = self->w->entities[i];
            if (!e || e == p)
                continue;
            v2 dir = Vector2Normalize(Vector2Subtract(p->pos, e->pos));
            EntityApplyForce(e, Vector2Scale(dir, e->spd));
        }
        WorldUpdate(self->w);
    } else {
        if (IsKeyPressed(KEY_SPACE)) {
            self->started = true;
        }
    }
    if (((Player *)p)->alive && self->started) {
        self->seconds += GetFrameTime();
    }
}
Screen *MainScreenNew(bool started) {
    MainScreen *self = MemAlloc(sizeof(MainScreen));
    self->started = started;
    self->base = ScreenNew();
    self->base.on_load = _msc_load;
    self->base.on_unload = _msc_unload;
    self->base.on_draw = _msc_draw;
    self->base.on_update = _msc_update;
    return (Screen *)self;
}
