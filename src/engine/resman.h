#pragma once

#include "types.h"
#include <raylib.h>

void ResManInit();
void ResManDispose();

typedef enum {
	RES_SOUND,
	RES_IMAGE,
	RES_TEXTURE,
	RES_FONT,
	RES_SHADER
} ResourceType;

bool ResManGetResource(const char* key, ResourceType type, void* out);
const char** ResManGetKeys(ResourceType type, i32 *count);
