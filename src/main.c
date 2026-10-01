#define HASHTABLE_IMPL
#include "types.h"
#include "engine/app.h"
#include "game/states/mmst.h"
#include <hashtable.h>

i32 main() {
    AppInit();
    AppSetState(MainMenuState_New(false));
    AppRun();
    return 0;
}
