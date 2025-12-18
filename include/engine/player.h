#pragma once

#include "engine/entity.h"

typedef struct _ep {
    Entity base;
    bool alive;
    bool was_onground;
    bool onground;
    bool jump_buf;
    i32 jumps_left;
    i32 jumps;
    f64 jump_timer;
    f64 jump_time;
    f32 jump_force;
    f32 stomp_force;
    f32 dash_force;
    i32 dashes_left;
    i32 dashes;
    bool dashing;
    f32 dash_timer;
    f32 dash_duration;
} Player;

Entity* PlayerNew(v2 pos, Color tint);
