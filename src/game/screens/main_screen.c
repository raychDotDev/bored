#include "game/screens/main_screen.h"
#include "engine/enemy.h"
#include "engine/entity.h"
#include "engine/player.h"
#include "engine/resman.h"
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
    Color palletes[3][3];
} MainScreen;

void _msc_update_camera(MainScreen *self, v2 targetPos) {
    v2 ss = {GetScreenWidth(), GetScreenHeight()};
    v2 sshalf = {ss.x / 2, ss.y / 2};
    self->camera.target =
        Vector2Lerp(self->camera.target, targetPos, 7.5f * GameGetFrameTime());
    self->camera.offset = sshalf;
    self->camera.rotation = 0.f;
    self->camera.zoom = fabsf(ss.y / self->w->size.y) * 0.5f;
}
void _msc_set_pallette(MainScreen *self, u32 index) {
    self->w->outer_color = self->palletes[index][0];
    self->w->inner_color = self->palletes[index][1];
    self->w->contrast_color = self->palletes[index][2];
}
void _msc_load(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    self->palletes[0][0] = BLACK;
    self->palletes[0][1] = (Color){227, 212, 212, 255};
    self->palletes[0][2] = (Color){133, 0, 0, 255};
    self->palletes[1][0] = (Color){197, 200, 181, 255};
    self->palletes[1][1] = (Color){253, 255, 254, 255};
    self->palletes[1][2] = (Color){94, 0, 0, 255};
    self->palletes[2][0] = (Color){0, 10, 0, 255};
    self->palletes[2][1] = (Color){238, 206, 0, 255};
    self->palletes[2][2] = (Color){224, 118, 0, 255};
    self->w = WorldCreate((v2){100, 40});
    _msc_set_pallette(self, 0);
    Entity *player =
        PlayerNew((v2){self->w->size.x * 0.5f, self->w->size.y * 0.5f},
                  self->w->outer_color);
    Entity *a = EnemyNew((v2){self->w->size.x * 0.1f, self->w->size.y * 0.1f},
                         self->w->contrast_color, self->w->outer_color);
    Entity *b = EnemyNew((v2){self->w->size.x * 0.9f, self->w->size.y * 0.1f},
                         self->w->contrast_color, self->w->outer_color);
    Enemy *be = ((Enemy *)b);
    be->dash_cd_timer = -be->dash_cd * .5f;
    WorldAddEntity(self->w, player);
    WorldAddEntity(self->w, a);
    WorldAddEntity(self->w, b);
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
    outer.a = 190;
    const char *secs = "%.1f";
    secs = TextFormat(secs, self->seconds);
    f32 ss = MeasureText(secs, ts);
    DrawText(secs, self->w->size.x / 2 - ss / 2, self->w->size.y / 2 - ts / 2,
             ts, outer);
    for (i32 i = 0; i < self->w->entities_count; i++) {
        Entity *e = self->w->entities[i];
		EntityDraw(e, self->w, GameGetFrameTime());
    }
    EndMode2D();
    if (!p->alive || !self->started) {
        GameCRTOn();
        DrawRectangleV((v2){}, (v2){GetScreenWidth(), GetScreenHeight()},
                       outer);
        f32 ts = 8 * self->camera.zoom;
        const char *text = "PRESS [SPACE] TO (RE)START";
        f32 text_s = MeasureText(text, ts);
        DrawText(text, ww / 2.f - text_s / 2, wh / 2, ts, self->w->inner_color);
        if (!p->alive) {
            const char *secs = "YOUR SCORE: %.3f SECONDS";
            secs = TextFormat(secs, self->seconds);
            f32 secs_s = MeasureText(text, ts);
            DrawText(secs, ww / 2.f - secs_s / 2, wh / 2 - ts, ts,
                     self->w->inner_color);
        }
    } else {
        GameCRTOff();
    }
	DrawFPS(10,10);
}
void _msc_update(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    Entity *p = self->w->entities[0];
    Sound sound;
    ResManGetSound("music", &sound);
    SetSoundVolume(sound, 0.3f);
    _msc_update_camera(self, p->pos);
    if (!((Player *)p)->alive) {
        if (IsSoundPlaying(sound)) {
            StopSound(sound);
        }
        if (IsKeyPressed(KEY_SPACE)) {
            GameSetScreen(MainScreenNew(true));
        }
    } else if (((Player *)p)->alive && self->started) {
        if (!IsSoundPlaying(sound)) {
            PlaySound(sound);
        }
        for (i32 i = 0; i < self->w->entities_count; i++) {
            Entity *e = self->w->entities[i];
            if (!e || e == p)
                continue;
            v2 dir = Vector2Normalize(Vector2Subtract(p->pos, e->pos));
            EntityApplyForce(e, Vector2Scale(dir, e->spd), GameGetFrameTime());
        }
        WorldUpdate(self->w, GameGetFrameTime());
    } else {
        if (IsKeyPressed(KEY_SPACE)) {
            self->started = true;
            PlaySound(sound);
        }
    }
    if (((Player *)p)->alive && self->started) {
        self->seconds += GameGetFrameTime();
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
