#pragma once

#include "engine/entity.h"
#include <raylib.h>

typedef struct _ee {
	Entity base;
	bool dashing;
	f32 dash_force;
	f32 dash_timer;
	f32 dash_time;
	f32 dash_cd;
	f32 dash_cd_timer;
	Entity*target;
	f32 dist_to_dash;
	Color tint2;
	Color tint3;
} Enemy;

Entity* EnemyNew(v2 pos, Color tint, Color tint2);
