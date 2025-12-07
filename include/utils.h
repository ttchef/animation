
#ifndef UTILS_H
#define UTILS_H

#include <glad/glad.h> 
#include <HandmadeMath.h>
#include <stdint.h> 

// Normal types
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

typedef u8 b8;
typedef size_t usize; // zig enjoyer?

// OpenGL
typedef GLubyte gl_u8;
typedef GLushort gl_u16;
typedef GLuint gl_u32;

typedef GLbyte gl_i8;
typedef GLshort gl_i16;
typedef GLint gl_i32;

#if defined(__clang__) || defined(__gcc__)
    #define STATIC_ASSERT _Static_assert
#else 
    #define STATIC_ASSERT static_assert 
#endif

#define TRUE 1 
#define FALSE 0 

typedef struct Color {
    f32 r;
    f32 g;
    f32 b;
    f32 a;
} Color;

#define COLOR(r, g, b, a) (Color){r, g, b, a}

typedef struct ColorHex {
    u8 r;
    u8 g;
    u8 b;
    u8 a;
} ColorHex;

#define COLOR_HEX(r, g, b, a) (ColorHex){r, g, b, a}

#endif
