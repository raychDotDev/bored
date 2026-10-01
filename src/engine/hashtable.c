#include "engine/hashtable.h"
#include <raylib.h>
#include <string.h>

#define HASH_TABLE_MAX_SIZE 8096

// djb33 hash function
u64 Hash(const char *s, u64 len) {
    u64 h = 5381;
    while (len--) {
        h += (h << 5);
        h ^= *s++;
    }
    return h;
}

HashTable *HTCreate() {
    HashTable *self = MemAlloc(sizeof(HashTable));
    self->items = MemAlloc(sizeof(HashTableItem *) * HASH_TABLE_MAX_SIZE);
    memset(self->items, 0, sizeof(HashTableItem *) * HASH_TABLE_MAX_SIZE);
    self->count = 0;
    return self;
}

void HTDestroy(HashTable *self) {
    if (!self)
        return;
    for (int i = 0; i < HASH_TABLE_MAX_SIZE; i++) {
        HashTableItem *item = self->items[i];
        if (!item)
            continue;
        while (item) {
            HashTableItem *next = item->next;
            if (item->key) {
                MemFree((char *)item->key);
            }
            if (item->value) {
                MemFree(item->value);
            }
            MemFree(item);
            item = next;
        }
    }
}

void *HTGet(HashTable *self, const char *key) {
    if (!self)
        return nullptr;
    u64 hash = Hash(key, strlen(key));
    u64 index = hash % HASH_TABLE_MAX_SIZE;
    HashTableItem *i = self->items[index];
    while (i && i->key && !TextIsEqual(i->key, key)) {
        i = i->next;
        if (!i)
            return nullptr;
    }
    if (!i || !i->value)
        return nullptr;
    return i->value;
}

// WARNING: value must be allocated in heap via MemAlloc for further dispose
void HTSet(HashTable *self, const char *key, void *value) {
    if (!self || !key)
        return;
    u64 hash = Hash(key, TextLength(key));
    u64 index = hash % HASH_TABLE_MAX_SIZE;
    HashTableItem *i = self->items[index];
    while (i && !TextIsEqual(i->key, key)) {
        i = i->next;
    }
    if (i) {
        i->value = value;
    } else {
        i = MemAlloc(sizeof(HashTableItem));
        i->key = MemAlloc(sizeof(char) * strlen(key) + 1);
        TextCopy((char *)i->key, key);
        i->value = value;
        if (self->items[index])
            i->next = self->items[index];
        self->items[index] = i;
    }
    self->count++;
}

void HTDelete(HashTable *self, const char *key) {
    if (!self || !key)
        return;
    u64 hash = Hash(key, strlen(key));
    u64 index = hash % HASH_TABLE_MAX_SIZE;
    HashTableItem *temp = nullptr;
    HashTableItem *i = self->items[index];
    while (i && i->key && !TextIsEqual(i->key, key)) {
        temp = i;
        i = i->next;
    }
    if (!i)
        return;
    if (i->value != nullptr) {
        MemFree(i->value);
    }
    MemFree((char *)i->key);
    if (temp)
        temp->next = i->next;
    else
        self->items[index] = i->next;
    MemFree(i);
    i = nullptr;
}

const char **HTGetKeys(HashTable *self, i32 *count) {
    u64 capacity = 1;
    const char **keys = MemAlloc(sizeof(char *) * capacity);
    memset(keys, 0, sizeof(char) * capacity);
    *count = 0;
    for (i32 i = 0; i < HASH_TABLE_MAX_SIZE; i++) {
        HashTableItem *item = self->items[i];
        if (!item)
            continue;
        while (item) {
            const char *key = MemAlloc(sizeof(char) * 128);
            TextCopy((char *)key, item->key);
            keys[*count] = MemAlloc(sizeof(char) * (strlen(key) + 1));
            TextCopy((char *)keys[*count], key);
            MemFree((char *)key);
            ++*count;
            if (*count == capacity)
                capacity *= 2;
            keys = MemRealloc(keys, sizeof(char *) * capacity);
            item = item->next;
        }
    }
    return keys;
}
