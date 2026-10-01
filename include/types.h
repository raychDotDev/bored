#pragma once

#include <raylib.h>
#include <stddef.h>
#include <stdint.h>

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef struct _vector2i {
	i32 x;
	i32 y;
} Vector2i;
typedef struct _vector3i {
	i32 x;
	i32 y;
	i32 z;
} Vector3i;

typedef Vector2 v2;
typedef Vector2i v2i;
typedef Vector3 v3;
typedef Vector3i v3i;
