#pragma once

#include "engine/world.h"
#include <raylib.h>
typedef struct WorldRenderer WorldRenderer;
typedef struct WorldRenderer {
    Camera2D camera;
    Vector2 target;
    f32 zoom, step;
} WorldRenderer;

WorldRenderer WorldRendererCreate(Vector2 pos);

void WorldRendererUpdate(WorldRenderer *self, World *ctx, f32 dt);
void WorldRendererRender(WorldRenderer *self, World *ctx);
