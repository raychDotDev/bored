#include "fixed.h"
#include <raylib.h>
#include <raymath.h>
#define GRAVITY 9.8f
typedef struct _p {
    v2 pos;
    v2 vel;
    v2 col_norm;
    f32 speed;
    f32 jump_force;
    f32 rad;
} Player;

typedef struct _e {
    v2 pos;
    v2 vel;
    v2 col_norm;
    f32 speed;
    f32 rad;
} Enemy;

typedef struct _t {
    Rectangle bounds;
} Tile;

v2i g_get_window_size() { return (v2i){GetScreenWidth(), GetScreenHeight()}; }
void setwindowsize(v2i size) { SetWindowSize(size.x, size.y); }
bool g_running = true;
void g_draw();
void g_update();
Camera2D cam;
const Rectangle world_bounds = {-400, -100, 800, 200};
Player p_init = {.pos = (v2){-5.f, 0},
                 .vel = (v2){0, 0},
                 .rad = 10.f,
                 .speed = 10.f,
                 .jump_force = 4.f};
Player p;
Enemy e_init = {
    .col_norm = {},
    .pos = {-80, -80},
    .rad = 12.f,
    .speed = 2.5f,
    .vel = {},
};
#define ENEMY_COUNT 2
Enemy e[2];
f32 jump_buf_timer_max = 0.1f;
bool jump_buf = false;
f32 jump_timer;
f32 p_max_vel = 5.f;
void g_reset();
bool pause = false;
bool stop;
bool first_start = true;
void g_init() {
    jump_timer = GetTime();
    v2i w_size = {720, 480};
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(w_size.x, w_size.y, "bored");
    SetTargetFPS(120);
    SetExitKey(KEY_NULL);
    g_reset();
    stop = true;
}
void g_dispose() {
    // TODO
    CloseWindow();
}
void g_stop() { g_running = false; }
void g_run() {
    g_init();
    MaximizeWindow();
    while (!WindowShouldClose() && g_running) {
        BeginDrawing();
        ClearBackground(BLACK);
        g_draw();
        EndDrawing();
        g_update();
    }
    g_dispose();
}
i32 main() {
    g_run();
    return 0;
}

f32 time = 0;
void g_draw() {
    BeginMode2D(cam);
    DrawRectangleRec(world_bounds, WHITE);
    DrawCircleV(p.pos, p.rad, BLACK);
    DrawText(TextFormat("%d", (i32)time), -10, -10, 20, BLACK);
    for (i32 i = 0; i < ENEMY_COUNT; i++) {
        DrawCircleV(e[i].pos, e[i].rad, MAROON);
    }
    EndMode2D();
    Color pause_col = {100, 100, 100, 150};
    v2i ss = g_get_window_size();
    if (pause) {
        DrawRectangle(0, 0, ss.x, ss.y, pause_col);
        const char *txt = TextFormat("PRESS [ESCAPE] TO RESUME");
        i32 fs = 40;
        i32 s = MeasureText(txt, fs);
        DrawText(txt, ss.x / 2 - s / 2, ss.y / 2 - fs / 2 - fs, fs, WHITE);
    } else if (stop) {
        DrawRectangle(0, 0, ss.x, ss.y, pause_col);
        if (!first_start) {
            const char *txt = TextFormat("YOUR SCORE: %d SECOND(S)", (i32)time);
            i32 fs = 40;
            i32 s = MeasureText(txt, fs);
            DrawText(txt, ss.x / 2 - s / 2, ss.y / 2 - fs / 2 - fs * 2, fs,
                     WHITE);
        } else {
            const char *txt[4] = {
                "[A]/[D] - MOVE;",
                "[SPACE] - jump/double jump;",
                "[S]+[SPACE] - stomp;",
                "[LSHIFT] - horizontal dash",
            };

            i32 fs = 40;
            for (i32 i = 0; i < 4; i++) {
                i32 s = MeasureText(txt[i], fs);
                DrawText(txt[i], ss.x / 2 - s / 2,
                         ss.y / 2 - (fs * 4) - fs * (i), fs, WHITE);
            }
        }
        const char *txt = "PRESS [ENTER] TO START";
        i32 fs = 40;
        i32 s = MeasureText(txt, fs);
        DrawText(txt, ss.x / 2 - s / 2, ss.y / 2 - fs / 2, fs, WHITE);
    }
}
void g_reset() {
    stop = false;
    pause = false;
    p = p_init;
    for (i32 i = 0; i < ENEMY_COUNT; i++) {
        *&e[i] = e_init;
        *&e[i].pos.x = world_bounds.x +
                       world_bounds.width / ENEMY_COUNT * (i + 0.5f) +
                       e->rad * 0.5f;
    }
    time = 0;
}
i32 djump_max = 1;
i32 djump_left = 0;
void g_update() {
    v2i ss = g_get_window_size();
    cam.offset = (v2){(f32)ss.x / 2.f, (f32)ss.y / 2.f};
    cam.target = Vector2Lerp(cam.target, p.pos, 0.4f);
    // cam.zoom = ;
    // cam.zoom =
    //     Lerp(cam.zoom, ss.y / (Vector2Distance(e.pos, p.pos) * 2.5f), 0.4f);
    cam.zoom = fabs(((f32)ss.x / (f32)world_bounds.x) * 0.6);
    cam.rotation = 0.f;
    if (stop) {
        if (IsKeyPressed(KEY_ENTER)) {
            g_reset();
            if (first_start)
                first_start = false;
        }
        return;
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        pause = !pause;
    }
    if (pause) {
        return;
    }
    time += GetFrameTime();
    i32 hor = IsKeyDown(KEY_D) - IsKeyDown(KEY_A);
    bool down = IsKeyDown(KEY_S);
    bool jump = IsKeyPressed(KEY_SPACE);
    if (jump && !down) {
        jump_timer = GetTime();
        jump_buf = true;
    }
    bool dash = IsKeyPressed(KEY_LEFT_SHIFT);
    const f32 eps = 0.001f;
    bool can_jump = GetTime() - jump_timer <= jump_buf_timer_max;
    if (!can_jump) {
        jump_buf = false;
    }
    p.vel.x = p.vel.x + hor * (dash ? p.speed * 2 : p.speed * GetFrameTime());
    f32 steer = 10.f;
    if (p.col_norm.y < 0) {
        djump_left = djump_max;
    }
    if (jump && down) {
        p.vel.y = p.jump_force * 2;
        jump_buf = false;
        jump_timer = 0.f;
    } else if ((p.col_norm.y < 0 && jump_buf && can_jump) ||
               (!down && djump_left > 0 && jump)) {
        p.vel.y = -p.jump_force;
        if (p.col_norm.y >= 0) {
            djump_left--;
        }
    } else if (p.col_norm.x != 0 && jump_buf && can_jump) {
        p.vel.x = (p.jump_force * 0.5f) * p.col_norm.x;
        p.vel.y = -p.jump_force;
    } else {
        p.vel.y = p.vel.y + GRAVITY * GetFrameTime();
    }
    if (hor == 0 && p.col_norm.y != 0)
        p.vel.x = Vector2Lerp(p.vel, Vector2Zero(), steer * GetFrameTime()).x;
    p.vel.x = Clamp(p.vel.x, -p_max_vel, p_max_vel);
    v2 next_pos = Vector2Add(p.pos, p.vel);
    if (next_pos.x - p.rad < world_bounds.x ||
        next_pos.x + p.rad >= world_bounds.x + world_bounds.width) {
        if (p.vel.x > 0) {
            p.col_norm.x = -1;
        } else if (p.vel.x < 0) {
            p.col_norm.x = 1;
        }
        p.vel.x = 0;
    } else {
        p.col_norm.x = 0;
    }
    if (next_pos.y - p.rad < world_bounds.y ||
        next_pos.y + p.rad >= world_bounds.y + world_bounds.height) {
        if (p.vel.y > 0) {
            p.col_norm.y = -1;
        } else if (p.vel.y < 0) {
            p.col_norm.y = 1;
        }
        p.vel.y = 0;
    } else {
        p.col_norm.y = 0;
    }
    next_pos = Vector2Clamp(
        next_pos,
        (v2){world_bounds.x + p.rad + eps, world_bounds.y + p.rad + eps},
        (v2){world_bounds.x + world_bounds.width - p.rad - eps,
             world_bounds.y + world_bounds.height - p.rad - eps});
    p.pos = next_pos;
    for (i32 i = 0; i < ENEMY_COUNT; i++) {
        Enemy enemy = e[i];
        v2 e_dir = Vector2Normalize(Vector2Subtract(p.pos, enemy.pos));
        enemy.vel.x += e_dir.x * enemy.speed * GetFrameTime();
        enemy.vel.y += e_dir.y * enemy.speed * GetFrameTime();
        v2 e_next_pos =
            Vector2Add(enemy.pos, Vector2Scale(enemy.vel, GetFrameTime()));
        if (e_next_pos.x + enemy.rad > world_bounds.x + world_bounds.width ||
            e_next_pos.x - enemy.rad < world_bounds.x) {
            enemy.vel.x = -enemy.vel.x * 0.5f;
        }
        if (e_next_pos.y + enemy.rad > world_bounds.y + world_bounds.height ||
            e_next_pos.y - enemy.rad < world_bounds.y) {
            enemy.vel.y = -enemy.vel.y * 0.5f;
        }
        e_next_pos =
            Vector2Add(enemy.pos, Vector2Scale(enemy.vel, enemy.speed));
        e_next_pos = Vector2Clamp(
            e_next_pos,
            (v2){world_bounds.x + enemy.rad, world_bounds.y + enemy.rad},
            (v2){world_bounds.x + world_bounds.width - enemy.rad,
                 world_bounds.y + world_bounds.height - enemy.rad});
        enemy.pos = e_next_pos;
        // enemy.speed = enemy.speed * (i32)((time / 10));
        *&e[i] = enemy;
        for (i32 j = 0; j < ENEMY_COUNT; j++) {
            if (&e[i] == &e[j])
                continue;
            f32 dist = Vector2Distance(e[i].pos, e[j].pos);
            bool col =
                CheckCollisionCircles(e[i].pos, e[i].rad, e[j].pos, e[j].rad);
            if (col && dist < e[i].rad + e[j].rad + eps) {
                v2 n = Vector2Normalize(Vector2Subtract(e[j].pos, e[i].pos));
                v2 n2 = Vector2Normalize(Vector2Subtract(e[i].pos, e[j].pos));
                f32 e1len = Vector2Length(e[i].vel);
                e[i].vel = Vector2Scale(n2, e1len);
                f32 e2len = Vector2Length(e[j].vel);
                e[j].vel = Vector2Scale(n, e2len);
            }
        }
        e_next_pos =
            Vector2Add(enemy.pos, Vector2Scale(enemy.vel, enemy.speed));
        if (CheckCollisionCircles(p.pos, p.rad, e[i].pos, e[i].rad)) {
            stop = true;
        }
    }
}
