#if !defined(BUILD_TYPES_H)

//////////////////////////////
//        Compilers         //
//////////////////////////////
#if !defined(COMPILER_MSVC)
#define COMPILER_MSVC 0
#endif

#if !defined(COMPILER_LLVM)
#define COMPILER_LLVM 0
#endif

#if !COMPILER_MSVC && !COMPILER_LLVM
#if defined(_MSC_VER)
#undef COMPILER_MSVC
#define COMPILER_MSVC 1
#else
// TODO(yuval, eran): Support More Compilers!
#undef COMPILER_LLVM
#define COMPILER_LLVM 1
#endif
#endif

#if COMPILER_MSVC
#include <intrin.h>
#elif COMPILER_LLVM
#include <x86intrin.h>
#else
#error Optimizations are not available for this compiler yet!!!
#endif

//////////////////////////////
//          Types           //
//////////////////////////////
#include <stdint.h>
#include <stddef.h>

#define build_internal static
#define build_global_variable static
#define build_local_persist static

// NOTE(yuval): To be used only in the same function
// where the array was defined or with arrays that are defined
// within structs
#define BuildArrayCount(Array) (sizeof(Array) / sizeof((Array)[0]))

typedef int8_t build_s8;
typedef int16_t build_s16;
typedef int32_t build_s32;
typedef int64_t build_s64;
typedef build_s32 build_b32;

typedef uint8_t build_u8;
typedef uint16_t build_u16;
typedef uint32_t build_u32;
typedef uint64_t build_u64;

typedef float build_f32;
typedef double build_f64;

typedef intptr_t build_smm;
typedef uintptr_t build_umm;

#define BUILD_TYPES_H
#endif


