#if !defined(YD_BUCKET_ARRAY)

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define yd_internal static
#define yd_global static
#define yd_local_persist static

typedef int8_t yd_s8;
typedef int16_t yd_s16;
typedef int32_t yd_s32;
typedef int64_t yd_s64;
typedef yd_s32 yd_b32;

typedef uint8_t yd_u8;
typedef uint16_t yd_u16;
typedef uint32_t yd_u32;
typedef uint64_t yd_u64;

typedef float yd_f32;
typedef double yd_f64;

typedef intptr_t yd_smm;
typedef uintptr_t yd_umm;
#define YD_TYPES
#endif // #if !defined(YD_TYPES)

#if !defined(YDAssert)
# define YDAssert(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#endif // #if !defined(YDAssert)

template <class t>
struct bucket
{
    t* Data;
    yd_umm Count;
    yd_umm Capacity;
    
    bucket<t>* Next;
};

template <class t>
struct bucket_array
{
    bucket<t>* Head;
    bucket<t>* Tail;
    
    yd_umm ItemsPerBucket;
    yd_umm Count;
    
    inline t& operator[](yd_umm Index);
};

//
// NOTE(yuval): Public API Inline Functions
//

template <class t>
yd_internal inline bucket_array<t>
MakeBucketArray(yd_umm ItemsPerBucket = 0)
{
    bucket_array<t> Result;
    Result.Head = 0;
    Result.Tail = 0;
    Result.ItemsPerBucket = ItemsPerBucket;
    Result.Count = 0;
    
    return Result;
}

template <class t>
yd_internal inline void
Append(bucket_array<t>* Array, t Value, memory_arena* Arena)
{
    bucket<t>* Tail = Array->Tail;
    
    if (!Tail || (Tail->Count == Tail->Capacity))
    {
        // NOTE(yuval): We need to allocate a new bucket
        Array->Tail = PushStruct(Arena, bucket<t>);
        
        if (Tail)
        {
            Tail->Next = Array->Tail;
        }
        else
        {
            Array->Head = Tail;
        }
        
        Tail = Array->Tail;
        
        if (Array->ItemsPerBucket == 0)
        {
            Array->ItemsPerBucket = 8;
        }
        
        Tail->Data = PushArray(Arena, t, Array->ItemsPerBucket);
        Tail->Capacity = Array->ItemsPerBucket;
    }
    
    Tail->Data[Tail->Count++] = Value;
    ++Array->Count;
}

template <class t>
inline t&
bucket_array<t>::operator[](yd_umm Index)
{
    bucket<t>* CurrBucket = Head;
    
    while ((Index > ItemsPerBucket) && CurrBucket)
    {
        CurrBucket = CurrBucket->Next;
        Index -= ItemsPerBucket;
    }
    
    // TODO(yuval):
    /*if (CurrBucket)
    {
    
    }*/
    
    t& Result = CurrBucket->Data[Index];
    return Result;
}

#endif // #if !defined(YD_BUCKET_ARRAY)