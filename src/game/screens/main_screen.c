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
    someone->spd = 430.f;
    someone->evwct = EVWCT_BOUNCE;
    someone->affected_by_gravity = false;
    Entity *somebody = MemAlloc(sizeof(Entity));
    *somebody = EntityNew();
    somebody->pos = (v2){self->w->size.x * 0.9f, self->w->size.y * 0.1f};
    somebody->rad = 3.f;
    somebody->tint = self->w->outer_color;
    somebody->spd = 390.f;
    somebody->evwct = EVWCT_BOUNCE;
    somebody->affected_by_gravity = false;
    WorldAddEntity(self->w, player);
    WorldAddEntity(self->w, someone);
    WorldAddEntity(self->w, somebody);
    self->camera = (Camera2D){};
}
void _msc_unload(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    WorldDestroy(self->w);
}
void _msc_draw(Screen *s) {
    MainScreen *self = (MainScreen *)s;

    DrawRectangleV((v2){}, (v2){GetScreenWidth(), GetScreenHeight()},
                   self->w->outer_color);
    BeginMode2D(self->camera);
    DrawRectangleV((v2){}, self->w->size, self->w->inner_color);
    for (i32 i = 0; i < self->w->entities_count; i++) {
        Entity *e = self->w->entities[i];
        DrawCircleV(e->pos, e->rad, e->tint);
    }
    EndMode2D();
}
void _msc_update(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    Entity *p = self->w->entities[0];
    _msc_update_camera(self, p->pos);
    if (((Player *)p)->alive) {
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
			GameSetScreen(MainScreenNew());
		}
	}
}
Screen *MainScreenNew() {
    MainScreen *self = MemAlloc(sizeof(MainScreen));
    self->base = ScreenNew();
    self->base.on_load = _msc_load;
    self->base.on_unload = _msc_unload;
    self->base.on_draw = _msc_draw;
    self->base.on_update = _msc_update;
    return (Screen *)self;
}
