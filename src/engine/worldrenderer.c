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
    self->camera.zoom = (wh * self->zoom) / ctx->size.y;
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

void WorldRendererUpdate(WorldRenderer *self, World *ctx, f32 dt) {
    WorldRendererUpdateCameraBounds(self, ctx);
    self->camera.target.x =
        FILerp(self->camera.target.x, self->target.x, self->step, dt);
    self->camera.target.y =
        FILerp(self->camera.target.y, self->target.y, self->step, dt);
}

void WorldRendererRender(WorldRenderer *self, World *ctx) {
    if (!self || !ctx)
        return;
    Color a = BLACK, b = {255, 243, 243, 255};
    Color x = ColorLerp(b, BLACK, 0.3f);
    BeginMode2D(self->camera);
    ClearBackground(a);
    Rectangle field = {0, 0, ctx->size.x, ctx->size.y};
    Rectangle fgp = field;
    Rectangle bgp = field;
    f32 fgz = 0.87;
    fgp.x = (fgp.x - self->camera.target.x) / fgz + self->camera.target.x;
    fgp.y = (fgp.y - self->camera.target.y) / fgz + self->camera.target.y;
    fgp.width /= fgz;
    fgp.height /= fgz;
    f32 z = 1.2f;
    bgp.x = (bgp.x - self->camera.target.x) / z + self->camera.target.x;
    bgp.y = (bgp.y - self->camera.target.y) / z + self->camera.target.y;
    bgp.width /= z;
    bgp.height /= z;

    DrawRectangleRec(fgp, b);
    DrawRectangleRec(field, a);
    DrawRectangleRec(bgp, b);

    Font f = GetFontDefault();
    const char *text = TextFormat("%.1f", ctx->time);
    f32 fsize = ctx->size.y / 3;
    f32 fspacing = fsize * 0.03;
    Vector2 size = MeasureTextEx(f, text, fsize, fspacing);
    DrawTextPro(f, text,
                (Vector2){
                    bgp.x + bgp.width / 2 - size.x / 2,
                    bgp.y + bgp.height / 2 - size.y / 2,
                },
                Vector2Zero(), 0.f, fsize, fspacing, x);
    for (i32 i = ctx->entities->size-1; i >= 0; i--) {
        Entity *e = ctx->entities->items[i];
        EntityDraw(e, ctx);
        DrawLineV(
            e->position,
            Vector2Add(e->position, Vector2Scale(Vector2Normalize(e->velocity), e->radius)),
            GREEN);
    }
    EndMode2D();
}
