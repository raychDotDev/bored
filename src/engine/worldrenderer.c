#include "worldrenderer.h"
#include "engine/entity.h"
#include "engine/entitylist.h"
#include "engine/mathutils.h"
#include <raylib.h>
#include <raymath.h>

void WorldRendererUpdateCameraBounds(WorldRenderer *self, World *ctx) {
    i32 ww, wh;
    ww = GetScreenWidth();
    wh = GetScreenHeight();
    self->camera.offset = (Vector2){ww / 2.f, wh / 2.f};
    self->camera.zoom = (wh * self->zoom) / ctx->size.y ;
}

WorldRenderer WorldRendererCreate(Vector2 pos) {
    WorldRenderer self = {};
    self.camera = (Camera2D){};
    self.camera.target = pos;
    self.target = pos;
    self.camera.rotation = 0.f;
    self.zoom = 1.f;
    self.step = 1.f;
    return self;
}

void WorldRendererUpdate(WorldRenderer *self, World* ctx, f32 dt) {
    WorldRendererUpdateCameraBounds(self, ctx);
    self->camera.target.x =
        FILerp(self->camera.target.x, self->target.x, self->step, dt);
    self->camera.target.y =
        FILerp(self->camera.target.y, self->target.y, self->step, dt);
}

void WorldRendererRender(WorldRenderer *self, World *ctx) {
    if (!self || !ctx)
        return;
    Color a = BLACK, b = WHITE, c = RED;
    BeginMode2D(self->camera);
    ClearBackground(a);
    DrawRectangle(0, 0, ctx->size.x, ctx->size.y, b);
    for (i32 i = 0; i < ctx->entities->size; i++) {
        Entity *e = ctx->entities->items[i];
        DrawCircleV(e->pos, e->rad, c);
    }
    EndMode2D();
}
