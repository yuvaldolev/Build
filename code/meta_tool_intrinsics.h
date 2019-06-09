#if !defined(METATOOLINTRINSICS_H)

#include <math.h>
#include <intrin.h>

inline s32
RoundReal32ToInt32(f32 real32)
{
    s32 result = _mm_cvtss_si32(_mm_set_ss(real32));
    return result;
}

#define METATOOLINTRINSICS_H
#endif
