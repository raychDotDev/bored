#pragma once

#include "engine/entity.h"
#include "fixed.h"
typedef struct _da {
    f32 dash_duration;
    f32 dash_timer;
    f32 dash_force;
    u32 dashes;
    u32 dashes_left;
    bool dashing;
} DashAbility;

DashAbility DashNew(f32 force, u32 max_dashes, f32 duration);
void DashRestore(DashAbility *self);
void DashReset(DashAbility *self);
void DashUpdate(DashAbility *self, Entity *target);
bool DashPerform(DashAbility *self, v2 dir, Entity *target);
