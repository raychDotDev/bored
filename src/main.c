#include <stdio.h>
#include <stdlib.h>
#define HASHTABLE_IMPL
#include "engine/app.h"
#include "game/states/mmst.h"
#include "types.h"
#include <hashtable.h>

i32 main(void) {
    AppInit();
    AppSetState(MainMenuState_New(false));
    AppRun();
    return 0;
}
