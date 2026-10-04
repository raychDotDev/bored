#pragma once

#include "types.h"
#include <raylib.h>

void ResManInit();
void ResManDispose();

typedef enum {
    RES_SOUND = 0,
    RES_IMAGE = 1 << 1,
    RES_TEXTURE = 1 << 2,
    RES_FONT = 1 << 3,
    RES_SHADER = 1 << 4,
    RES_LANG = 1 << 5
} ResourceType;

char *ResManGetLocaleString(const char *lang, const char *key, i32 *length);
bool ResManGetResource(const char *key, ResourceType type, void *out);
const char **ResManGetKeys(ResourceType type, i32 *count);
