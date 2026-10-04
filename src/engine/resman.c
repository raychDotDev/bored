#include "resman.h"
#include "hashtable.h"
#include "types.h"
#include <raylib.h>

HashTable *imageMap;
HashTable *textureMap;
HashTable *soundMap;
HashTable *shaderMap;
HashTable *fontMap;
HashTable *langMap;

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
        } else if (TextIsEqual(ext, ".locale")) {
            HashTable *table = HTCreate();
            char *text = LoadFileText(path);
            i32 lineCount = 0;
            char **lines = LoadTextLines(text, &lineCount);
            for (i32 i = 0; i < lineCount; i++) {
                char *line = lines[i];
                i32 c = 0;
                char **res = TextSplit(line, '=', &c);
                if (c != 2)
                    continue;
                char *value = MemAlloc(sizeof(char) * TextLength(res[1]));
                TextCopy(value, res[1]);
                HTSet(table, res[0], value);
            }
            if (table->count == 0) {
                HTDestroy(table);
                continue;
            }
            UnloadTextLines(lines, lineCount);
            UnloadFileText(text);
            HTSet(langMap, name, table);
            // TODO: NOT IMPLEMENTED
        }
    }
    TraceLog(LOG_INFO, "RESMAN: Loaded %d images", imageMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d textures", textureMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d shaders", shaderMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d fonts", fontMap->count);
    TraceLog(LOG_INFO, "RESMAN: Loaded %d sounds", soundMap->count);
    i32 langLines = 0;
    i32 count = 0;
    const char **k = HTGetKeys(langMap, &count);
    for (i32 i = 0; i < count; i++) {
        langLines += ((HashTable *)HTGet(langMap, k[i]))->count;
    }
    TraceLog(LOG_INFO, "RESMAN: Loaded %d locale tables, %d lines total",
             langMap->count, langLines);
    TraceLog(LOG_INFO, "RESMAN: Available locales:");
    for (i32 i = 0; i < count; i++) {
        TraceLog(LOG_INFO, "\t\t%s", k[i]);
    }
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
    langMap = HTCreate();
    TraceLog(LOG_INFO, "RESMAN: Locale table was created");
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
    HTDestroy(imageMap);
    HTDestroy(textureMap);
    MemFree(keys);
    keys = HTGetKeys(soundMap, &count);
    for (i32 i = 0; i < count; i++) {
        Sound *s = HTGet(soundMap, keys[i]);
        UnloadSound(*s);
    }
    HTDestroy(soundMap);
    MemFree(keys);
    keys = HTGetKeys(shaderMap, &count);
    for (i32 i = 0; i < count; i++) {
        Shader *s = HTGet(shaderMap, keys[i]);
        UnloadShader(*s);
    }
    HTDestroy(shaderMap);
    MemFree(keys);
    keys = HTGetKeys(fontMap, &count);
    for (i32 i = 0; i < count; i++) {
        Font *s = HTGet(fontMap, keys[i]);
        UnloadFont(*s);
    }
    HTDestroy(fontMap);
    MemFree(keys);
    keys = HTGetKeys(langMap, &count);
    for (i32 i = 0; i < count; i++) {
        HashTable *s = HTGet(langMap, keys[i]);
        HTDestroy(s);
    }
    HTDestroy(langMap);
    MemFree(keys);
    TraceLog(LOG_INFO, "RESMAN: Unloaded asset tables");
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

const char **ResManGetFontKeys(i32 *count) { return HTGetKeys(fontMap, count); }

char *ResManGetLocaleString(const char *lang, const char *key, i32 *length) {
    HashTable *res = HTGet(langMap, lang);
    if (!res)
        return nullptr;
    char *text = HTGet(res, key);
    if (length != nullptr) {
        *length = TextLength(text);
    }
    return text;
}
bool ResManGetResource(const char *key, ResourceType type, void *out) {
    switch (type) {
    case RES_SOUND: {
        return ResManGetSound(key, out);
    } break;
    case RES_IMAGE: {
        return ResManGetImage(key, out);
    } break;
    case RES_TEXTURE: {
        return ResManGetTexture(key, out);
    } break;
    case RES_FONT: {
        return ResManGetFont(key, out);
    } break;
    case RES_SHADER: {
        return ResManGetShader(key, out);
    } break;
    default:
        return false;
    }
}
const char **ResManGetKeys(ResourceType type, i32 *count) {
    switch (type) {
    case RES_SOUND: {
        return HTGetKeys(soundMap, count);
    } break;
    case RES_IMAGE: {
        return HTGetKeys(imageMap, count);
    } break;
    case RES_TEXTURE: {
        return HTGetKeys(textureMap, count);
    } break;
    case RES_FONT: {
        return HTGetKeys(fontMap, count);
    } break;
    case RES_SHADER: {
        return HTGetKeys(shaderMap, count);
    } break;
    case RES_LANG: {
        return HTGetKeys(langMap, count);
    } break;
    default:
        return nullptr;
    }
}
