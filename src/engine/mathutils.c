#include "mathutils.h"
#include <math.h>
#include <raymath.h>

f32 FILerp(f32 a, f32 b, f32 step, f32 dt) {
    return step == 1.f ? b : Lerp(a, b, 1.f - powf(step, dt));
}
