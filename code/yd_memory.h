#if !defined(YD_MEMORY)

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define internal_yd static
#define global_variable_yd static
#define local_persist_yd static

typedef int8_t s8_yd;
typedef int16_t s16_yd;
typedef int32_t s32_yd;
typedef int64_t s64_yd;
typedef s32_yd b32_yd;

typedef uint8_t u8_yd;
typedef uint16_t u16_yd;
typedef uint32_t u32_yd;
typedef uint64_t u64_yd;

typedef float f32_yd;
typedef double f64_yd;

typedef intptr_t smm_yd;
typedef uintptr_t umm_yd;
#define YD_TYPES
#endif

#if !defined(AssertYD)
# define AssertYD(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#endif

struct memory_arena
{
    u8_yd* Base;
    size_t Size;
    size_t Used;
    
    s32_yd TempCount;
};

struct temporary_memory
{
    memory_arena* Arena;
    size_t Used;
}

inline void InitializeArena(memory_arena* Arena, void* Base, size_t Size);
inline void SubArena(memory_arena* Result, memory_arena* Arena,
                     size_t Size, size_t Alignment = 16);
inline void Clear(memory_arena* Arena);
inline size_t GetAlignmentOffset(memory_arena* Arena, size_t Alignment);
inline size_t GetEffectiveSizeFor(memory_arena* Arena, size_t SizeInit, size_t Alignment);
inline void* PushSize_(memory_arena* Arena, size_t SizeInit, size_t Alignment = 4)

#if !defined(PushSize)
# define PushSize(Arena, Size, ...) PushSize_(Arena, Size, ## __VA_ARGS__)
#endif

#if !defined(PushStruct)
# define PushStruct(Arena, Type, ...) (Type*)PushSize_(Arena, sizeof(Type), ## __VA_ARGS__)
#endif

#if !defined (PushArray)
# define PushArray(Arena, Type, Count, ...) (Type*)PushSize_(Arena, (Count) * sizeof(Type), ## __VA_ARGS__)
#endif

inline temporary_memory BeginTemporaryMemory(memory_arena* Arena);
inline void EndTemporaryMemory(temporary_memory TempMem);
inline void CheckArena(memory_arena* Arena);
inline size_t GetArenaSizeRemaining(memory_arena* Arena, size_t Alignemnt = 4);
inline b32_yd ArenaHasRoomFor(memory_arena* Arena, size_t SizeInit, size_t Alignment = 4);

#if defined(YD_MEMORY_IMPLEMENTATION)

//
// NOTE(yuval): Arena / SubArena Initialization And Clearing
//

inline void
InitializeArena(memory_arena* Arena, void* Base, size_t Size)
{
    Arena->Base = (u8_yd*)Base;
    Arena->Size = Size;
    Arena->Used = 0;
    Arena->TempCount = 0;
}

inline void
SubArena(memory_arena* Result, memory_arena* Arena,
         size_t Size, size_t Alignment)
{
    Result->Base = (u8_yd*)PushSize(Arena, Size, Alignment);
    Result->Size = Size;
    Result->Used = 0;
    Result->TempCount = 0;
}

inline void
Clear(memory_arena* Arena)
{
    InitializeArena(Arena, Arena->Base, Arena->Size);
}

//
// NOTE(yuval): Arena Pushing And Alignment
//

inline size_t
GetAlignmentOffset(memory_arena* Arena, size_t Alignment)
{
    size_t AlignmentOffset = 0;
    size_t TailPointer = (size_t)Arena->Base + Arena->Used;
    size_t AlignmentMask = Alignment - 1;
    
    if (ResultPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (TailPointer & AlignmentMask);
    }
    
    return AlignmentOffset;
}

inline size_t
GetEffectiveSizeFor(memory_arena* Arena, size_t SizeInit, size_t Alignment)
{
    size_t AlignmentOffset = GetAlignmentOffset(Arena, Alignment);
    size_t Size = SizeInit + AlignmentOffset;
    AssertYD(Size >= SizeInit);
    
    return Size;
}

inline void*
PushSize_(memory_arena* Arena, size_t SizeInit, size_t Alignment)
{
    size_t Size = GetEffectiveSizeFor(Arena, SizeInit, Alignment);
    
    AssertYD((Arena->Used + Size) <= Arena->Size);
    
    size_t AlignmentOffset = GetAlignmentOffset(Arena, Alignment);
    void* Result = Arena->Base + Arena->Used + AlignmentOffset;
    Arena->Used += Size;
    
    return Result;
}

//
// NOTE(yuval): Temporary Arena Memory
//

inline temporary_memory
BeginTemporaryMemory(memory_arena* Arena)
{
    temporary_memory Result;
    
    Result.Arena = Arena;
    Result.Used = Arena->Used;
    
    ++Arena->TempCount;
    
    return Result;
}

inline void
EndTemporaryMemory(temporary_memory TempMem)
{
    memory_arena* Arena = TempMem.Arena;
    
    AssertYD(Arena->Used >= TempMem.Used);
    Arena->Used = TempMem.Used;
    
    AssertYD(Arena->TempCount > 0);
    --Arena->TempCount;
}

inline void
CheckArena(memory_arena* Arena)
{
    AssertYD(Arena->TempCount == 0);
}

//
// NOTE(yuval): Arena Size
//

inline void
GetArenaSizeRemaining(memory_arena* Arena, size_t Alignment)
{
    size_t Result = Arena->Size - (Arena->Used + GetAlignmentOffset(Arena, Alignment));
    return Result;
}

inline b32_yd
ArenaHasRoomFor(memory_arena* Arena, size_t SizeInit, size_t Alignment)
{
    size_t Size = GetEffectiveSizeFor(Arena, SizeInit, Alignment);
    b32_yd Result = ((Arena->Used + Size) <= Arena->Size);
    
    return Result;
}

#endif

#define YD_MEMORY
#endif