#if !defined(BUILD_INTRINSICS_H)

#include <math.h>

inline s32
round_f32_to_s32(f32 value) {
    s32 result = (s32)roundf(value);
    return result;
}

inline u32
round_f32_to_u32(f32 value) {
    u32 result = (u32)roundf(value);
    return result;
}

inline s32
truncate_f32_to_s32(f32 value) {
    s32 result = (s32)value;
    return result;
}

inline u32
safe_truncate_to_u32(u64 value) {
    // TODO(yuval): Defines for size limits
    ASSERT(value <= 0xFFFFFFFF);
    return (u32)value;
}

#if COMPILER_MSVC
# define COMPLETE_PREVIOUS_READS_BEFORE_FUTURE_READS(...) _ReadBarrier()
# define COMPLETE_PREVIOUS_WRITES_BEFORE_FUTURE_WRITES(...) _WriteBarrier()

inline u32
atomic_compare_exchange_u32(volatile u32* value, u32 new_value, u32 expected_value) {
    u32 result = _InterlockedCompareExchange((volatile long*)value, new_value, expecte_value);
    return result;
}

inline u64
atomic_exchange_u64(volatile u64* value, u64 new_value) {
    u64 result = _InterlockedExchange64((volatile __int64*)value, new_value);
    return result;
}

inline u64
atomic_add_u64(volatile u64* value, u64 addend) {
    // NOTE(yuval): Returns the original value _prior_ to the addition
    u64 result = _InterlockedExchangeAdd64((volatile __int64*)value, addend);
    return result;
}

inline u32
get_thread_id() {
    u8* thread_local_storage = (u8*)__readgsqword(0x30);
    u32 thread_id = *(u32*)(thread_local_storage + 0x48);
    
    return thread_id;
}
#elif COMPILER_LLVM
# define COMPLETE_PREVIOUS_READS_BEFORE_FUTURE_READS(...) asm volatile("" ::: "memory")
# define COMPLETE_PREVIOUS_WRITES_BEFORE_FUTURE_WRITES(...) asm volatile("" ::: "memory")

inline u32
atomic_compare_exchange_u32(volatile u32* value, u32 new_value, u32 expected_value) {
    u32 result = __sync_val_compare_and_swap(value, expected_value, new_value);
    return result;
}

inline u64
atomic_exchange_u64(volatile u64* value, u64 new_value) {
    u64 result = __sync_lock_test_and_set(value, new_value);
    return result;
}

inline u64
atomic_add_u64(volatile u64* value, u64 addend) {
    // NOTE(yuval): Returns the original value _prior_ to the addition
    u64 result = __sync_fetch_and_add(value, addend);
    return result;
}

inline u32
get_thread_id() {
    u32 thread_id;
    
#if defined(__APPLE__) && defined(__x86_64__)
    asm("mov %%gs:0x00,%0" : "=r"(thread_id));
#elif defined(__i386__)
    asm("mov %%gs:0x08,%0" : "=r"(thread_id));
#elif defined(__x86_64__)
    asm("mov %%fs:0x10,%0" : "=r"(thread_id));
#else
# error Unsupported Architecture
#endif // #if defined(__APPLE__) && defined(__x86_64__)
    
    return thread_id;
}
#else
// TODO(yuval): More compilers / platforms!
#endif // #if COMPILER_MSVC

#define BUILD_INTRINSICS_H
#endif // #if !defined(BUILD_INTRINSICS_H)
