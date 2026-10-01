#pragma once

#include "types.h"
#include <raylib.h>

void ResManInit();
void ResManDispose();

bool ResManGetImage(const char *key, Image *out);
bool ResManGetFont(const char *key, Font*out);
bool ResManGetTexture(const char *key, Texture2D *out);
bool ResManGetSound(const char *key, Sound *out);
bool ResManGetShader(const char *key, Shader *out);
const char **ResManGetSoundKeys(i32 *count);
const char **ResManGetShaderKeys(i32 *count);
const char **ResManGetImageKeys(i32 *count);
const char **ResManGetFontKeys(i32 *count);
const char **ResManGetTexutreKeys(i32 *count);
