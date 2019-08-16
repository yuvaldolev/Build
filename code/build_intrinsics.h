#if !defined(BUILD_INTRINSICS_H)

#include <math.h>

inline s32
RoundF32ToS32(f32 Value)
{
    s32 Result = (s32)roundf(Value);
    return Result;
}

inline u32
RoundF32ToU32(f32 Value)
{
    u32 Result = (u32)roundf(Value);
    return Result;
}

inline s32
TruncateF32ToS32(f32 Value)
{
    s32 Result = (s32)Value;
    return Result;
}

inline u32
SafeTruncateToU32(u64 Value)
{
    // TODO(yuval & eran): Defines for size limits
    Assert(Value <= 0xFFFFFFFF);
    return (u32)Value;
}

#if COMPILER_MSVC
# define CompletePreviousReadsBeforeFutureReads(...) _ReadBarrier()
# define CompletePreviousWritesBeforeFutureWrites(...) _WriteBarrier()

inline u32
AtomicCompareExchangeU32(volatile u32* Value, u32 New, u32 Expected)
{
    u32 Result = _InterlockedCompareExchange((volatile long*)Value, New, Expected);
    return Result;
}

inline u64
AtomicExchangeU64(volatile u64* Value, u64 New)
{
    u64 Result = _InterlockedExchange64((volatile __int64*)Value, New);
    return Result;
}

inline u64
AtomicAddU64(volatile u64* Value, u64 Addend)
{
    // NOTE(yuval): Returns the original value _prior_ to the addition
    u64 Result = _InterlockedExchangeAdd64((volatile __int64*)Value, Addend);
    return Result;
}

inline u32
GetThreadID()
{
    u8* ThreadLocalStorage = (u8*)__readgsqword(0x30);
    u32 ThreadID = *(u32*)(ThreadLocalStorage + 0x48);
    
    return ThreadID;
}
#elif COMPILER_LLVM
# define CompletePreviousReadsBeforeFutureReads asm volatile("" ::: "memory")
# define CompletePreviousWritesBeforeFutureWrites asm volatile("" ::: "memory")

inline u32
AtomicCompareExchangeU32(volatile u32* Value, u32 New, u32 Expected)
{
    u32 Result = __sync_val_compare_and_swap(Value, New, Expected);
    return Result;
}

inline u64
AtomicExchangeU64(volatile u64* Value, u64 New)
{
    u64 Result = __sync_lock_test_and_set(Value, New);
    return Result;
}

inline u64
AtomicAddU64(volatile u64* Value, u64 Addend)
{
    // NOTE(yuval): Returns the original value _prior_ to the addition
    u64 Result = __sync_fetch_and_add(Value, Addend);
    return Result;
}

inline u32
GetThreadID()
{
    u32 ThreadID;
    
#if defined(__APPLE__) && defined(__x86_64__)
    asm("mov %%gs:0x00,%0" : "=r"(ThreadID));
#elif defined(__i386__)
    asm("mov %%gs:0x08,%0" : "=r"(ThreadID));
#elif defined(__x86_64__)
    asm("mov %%fs:0x10,%0" : "=r"(ThreadID));
#else
# error Unsupported architecture
#endif // #if defined(__APPLE__) && defined(__x86_64__)
    
    return ThreadID;
}
#else
// TODO(yuval): More compilers / platforms!
#endif // #if COMPILER_MSVC

#define BUILD_INTRINSICS_H
#endif // #if !defined(BUILD_INTRINSICS_H)
