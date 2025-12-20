#pragma once

#include "engine/entity.h"
typedef struct _ed {
    Entity base;
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
    Entity *target;
    bool request_mleft;
    bool request_mright;
    bool request_dash;
    bool request_jump;
    bool request_stomp;
} Doppel;

Entity *DoppelNew(v2 pos, Color tint);
