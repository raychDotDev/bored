#pragma once

#include "types.h"
typedef struct Ability Ability;
typedef struct LivingEntity LivingEntity;

typedef struct Ability {
	bool (*canUse)(Ability* self, LivingEntity* user);
	f32 cooldown;
} Ability;
