#if !defined(META_TOOL_INTRINSICS_H)

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

#define META_TOOL_INTRINSICS_H
#endif
