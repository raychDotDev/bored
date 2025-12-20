#include "engine/dash_ability.h"
#include "engine/entity.h"

DashAbility DashNew(f32 force, u32 max_dashes, f32 duration) {
    DashAbility self = {};
    self.dash_force = 150.f;
    self.dashes = 3;
    self.dashes_left = 3;
    self.dashing = false;
    self.dash_duration = 0.10f;
    self.dash_timer = GetTime();
    return self;
}

void DashReset(DashAbility *self) {
    self->dashing = false;
    self->dash_timer = GetTime();
}

void DashUpdate(DashAbility *self, Entity *target) {
    if (GetTime() - self->dash_timer > self->dash_duration) {
		DashReset(self);
    }
}

void DashRestore(DashAbility *self) { 
	self->dashes_left = self->dashes; 
	
}
bool DashPerform(DashAbility *self, v2 dir, Entity *target) {
    if (!self->dashing && self->dashes_left > 0) {
        if (!dir.x && !dir.y) {
            return false;
        }
        if (dir.x) {
            target->vel.x = dir.x * self->dash_force;
        }
        if (dir.y) {
            target->vel.y = dir.y * self->dash_force;
        }
        self->dashing = true;
        self->dash_timer = GetTime();
        self->dashes_left--;
        return true;
    }
    return false;
}
