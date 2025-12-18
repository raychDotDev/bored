#include "game/screens/main_screen.h"
#include "engine/entity.h"
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
    self->camera.target = targetPos;
    self->camera.offset = sshalf;
    self->camera.rotation = 0.f;
    self->camera.zoom = fabsf(self->w->size.y / ss.y) * 10.f;
}
void _msc_load(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    self->w = WorldCreate((v2){200, 100});
    Entity *player = MemAlloc(sizeof(Entity));
    *player = EntityNew();
    player->pos = (v2){10, 10};
    player->rad = 2.f;
    player->tint = RED;
    player->collides_e = false;
    // player->evect = EVECT_BOUNCE;
    // player->evwct = EVWCT_BOUNCE;
    Entity *someone = MemAlloc(sizeof(Entity));
    *someone = EntityNew();
    someone->pos = (v2){30, 30};
    someone->rad = 2.f;
    someone->tint = GREEN;
    someone->evwct = EVWCT_BOUNCE;
    Entity *somebody = MemAlloc(sizeof(Entity));
    *somebody = EntityNew();
    somebody->pos = (v2){50, 50};
    somebody->rad = 2.f;
    somebody->tint = BLUE;
    // somebody->evect = EVECT_BOUNCE;
    somebody->evwct = EVWCT_BOUNCE;
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
    Entity *player = self->w->entities[0];
    DrawText(TextFormat("VELSELF: %f %f", player->vel.x, player->vel.y), 10, 10,
             20, WHITE);
    Entity *other = self->w->entities[1];
    DrawText(TextFormat("VELOTHER: %f %f", other->vel.x, other->vel.y), 10, 30,
             20, WHITE);
    DrawFPS(10, 50);
}
i32 fps = 240;
void _msc_update(Screen *s) {
    MainScreen *self = (MainScreen *)s;
    Entity *p = self->w->entities[0];
    _msc_update_camera(self, Vector2Scale(self->w->size, 0.5f));
    if (p) {
        v2 dir = Vector2Normalize((v2){
            IsKeyDown(KEY_D) - IsKeyDown(KEY_A),
            IsKeyDown(KEY_S) - IsKeyDown(KEY_W),
        });
        v2 force = Vector2Scale(dir, p->spd);
        EntityApplyForce(p, force);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        SetTargetFPS((fps = fps == 240 ? 60 : 240));
    }
	for (i32 i = 0; i < self->w->entities_count; i++) {
		Entity* e = self->w->entities[i];
		if (!e || e == p) continue;
		v2 dir = Vector2Normalize(Vector2Subtract(p->pos,e->pos));
		EntityApplyForce(e, Vector2Scale(dir, e->spd));
	}
    WorldUpdate(self->w);
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
