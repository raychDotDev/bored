#include "resman.h"
#include "hashtable.h"
#include "types.h"
#include <raylib.h>

HashTable *imageMap;
HashTable *textureMap;
HashTable *soundMap;
HashTable *shaderMap;
HashTable *fontMap;

void _rmLoad() {
    const char *cwd = GetWorkingDirectory();
    cwd = TextFormat("%s/assets", cwd);
    FilePathList list = LoadDirectoryFilesEx(cwd, nullptr, true);
    for (i32 i = 0; i < list.count; i++) {
        const char *path = list.paths[i];
        const char *nameRaw = GetFileName(path);
        const char *name = GetFileNameWithoutExt(path);
        const char *ext = GetFileExtension(nameRaw);
        if (TextIsEqual(ext, ".png")) {
            Image *img = MemAlloc(sizeof(Image));
            *img = LoadImage(path);
            HTSet(imageMap, name, img);
            Texture2D *tex = MemAlloc(sizeof(Texture2D));
            *tex = LoadTextureFromImage(*img);
            HTSet(textureMap, name, tex);
        } else if (TextIsEqual(ext, ".wav")) {
            Sound *s = MemAlloc(sizeof(Sound));
            *s = LoadSound(path);
            HTSet(soundMap, name, s);
        } else if (TextIsEqual(ext, ".glsl")) {
            Shader *s = MemAlloc(sizeof(Shader));
            *s = LoadShader(nullptr, path);
            HTSet(shaderMap, name, s);
        } else if (TextIsEqual(ext, ".ttf") || TextIsEqual(ext, ".otf")) {
            Font *f = MemAlloc(sizeof(Font));
            i32 codepoints[0x0500];
            for (i32 i = 0; i < 0x0500; i++)
                codepoints[i] = i;
            *f = LoadFontEx(path, 60, codepoints,
                            sizeof(codepoints) / sizeof(codepoints[0]));
            HTSet(fontMap, name, f);
        }
    }
    TraceLog(LOG_INFO, "RESMAN: Loaded %d images", imageMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d textures", textureMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d shaders", shaderMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d fonts", fontMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d sounds", soundMap->count);
    UnloadDirectoryFiles(list);
}
void ResManInit() {
    imageMap = HTCreate();
    TraceLog(LOG_INFO, "RESMAN: Image table was created");
    textureMap = HTCreate();
    TraceLog(LOG_INFO, "RESMAN: Texture table was created");
    soundMap = HTCreate();
    TraceLog(LOG_INFO, "RESMAN: Sound table was created");
    shaderMap = HTCreate();
    TraceLog(LOG_INFO, "RESMAN: Shader table was created");
    fontMap = HTCreate();
    TraceLog(LOG_INFO, "RESMAN: Font table was created");
    _rmLoad();
}
void ResManDispose() {
    i32 count = 0;
    const char **keys = HTGetKeys(imageMap, &count);
    for (i32 i = 0; i < count; i++) {
        Image *img = HTGet(imageMap, keys[i]);
        UnloadImage(*img);
        Texture *tex = HTGet(textureMap, keys[i]);
        UnloadTexture(*tex);
    }
    MemFree(keys);
    keys = HTGetKeys(soundMap, &count);
    for (i32 i = 0; i < count; i++) {
        Sound *s = HTGet(soundMap, keys[i]);
        UnloadSound(*s);
    }
    MemFree(keys);
    keys = HTGetKeys(shaderMap, &count);
    for (i32 i = 0; i < count; i++) {
        Shader *s = HTGet(shaderMap, keys[i]);
        UnloadShader(*s);
    }
    MemFree(keys);
    keys = HTGetKeys(fontMap, &count);
    for (i32 i = 0; i < count; i++) {
        Font *s = HTGet(fontMap, keys[i]);
        UnloadFont(*s);
    }
    MemFree(keys);
    HTDestroy(imageMap);
    HTDestroy(textureMap);
    HTDestroy(soundMap);
    HTDestroy(fontMap);
    HTDestroy(shaderMap);
    TraceLog(LOG_INFO, "RESMAN: Unloaded asset tables successfully");
}

bool ResManGetShader(const char *key, Shader *out) {
    Shader *res = HTGet(shaderMap, key);
    if (res) {
        *out = *res;
        return true;
    }
    return false;
}
bool ResManGetSound(const char *key, Sound *out) {
    Sound *res = HTGet(soundMap, key);
    if (res) {
        *out = *res;
        return true;
    }
    return false;
}
bool ResManGetImage(const char *key, Image *out) {
    Image *res = HTGet(imageMap, key);
    if (res) {
        *out = *res;
        return true;
    }
    return false;
}
bool ResManGetTexture(const char *key, Texture2D *out) {
    Texture *res = HTGet(textureMap, key);
    if (res) {
        *out = *res;
        return true;
    }
    return false;
}

bool ResManGetFont(const char *key, Font *out) {
    Font *res = HTGet(fontMap, key);
    if (res) {
        *out = *res;
        return true;
    }
    return false;
}

const char **ResManGetTexutreKeys(i32 *count) {
    return HTGetKeys(textureMap, count);
}
const char **ResManGetSoundKeys(i32 *count) {
    return HTGetKeys(soundMap, count);
}
const char **ResManGetShaderKeys(i32 *count) {
    return HTGetKeys(shaderMap, count);
}
const char **ResManGetImageKeys(i32 *count) {
    return HTGetKeys(imageMap, count);
}
const char **ResManGetFontKeys(i32 *count) { return HTGetKeys(fontMap, count); }
