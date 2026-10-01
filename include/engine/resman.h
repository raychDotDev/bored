#pragma once

#include "fixed.h"
#include <raylib.h>

void ResManInit();
void ResManDispose();

bool ResManGetImage(const char *key, Image *out);
bool ResManGetTexture(const char *key, Texture2D *out);
bool ResManGetSound(const char *key, Sound *out);
bool ResManGetShader(const char *key, Shader *out);
const char **ResManGetKeys(i32 *count);
// TODO
// Font ResManGetFont();
