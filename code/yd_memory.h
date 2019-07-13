#if !defined(YD_MEMORY)

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define yd_internal static
#define yd_global_variable static
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
#endif

#if !defined(YDAssert)
# define YDAssert(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#endif

#if !defined(YDMinimum)
#define YDMinimum(A, B) (((A) < (B)) ? (A) : (B))
#endif

#if !defined(YDMaximum)
# define YDMaximum(A, B) (((A) > (B)) ? (A) : (B))
#endif

enum memory_block_flags
{
    MemoryBlockFlag_NotRestored = 0x1,
    MemoryBlockFlag_OverflowCheck = 0x2,
    MemoryBlockFlag_UnderflowCheck = 0x4
};

struct memory_block
{
    yd_u64 Flags;
    
    yd_u8* Base;
    size_t Size;
    size_t Used;
    
    memory_block* Prev;
};

struct memory_arena
{
    memory_block* CurrentBlock;
    size_t MinimumBlockSize;
    
    yd_u64 AllocationFlags;
    yd_s32 TempCount;
};

struct temporary_memory
{
    memory_arena* Arena;
    memory_block* Block;
    size_t Used;
};

enum arena_push_flags
{
    ArenaFlag_ClearToZero = 0x1
};

struct arena_push_params
{
    yd_u32 Flags;
    yd_u32 Alignment;
};

struct arena_bootstrap_params
{
    yd_u64 AllocationFlags;
    size_t MinimumBlockSize;
};

//
// NOTE(yuval): Memory Blocks & Allocation / Deallocation
//

#define ALLOCATE_MEMORY(Name) memory_block* Name(size_t Size, u64 Flags)
typedef ALLOCATE_MEMORY(arena_allocate_memory);

#define DEALLOCATE_MEMORY(Name) void Name(memory_block* Block)
typedef DEALLOCATE_MEMORY(arena_deallocate_memory);

yd_global_variable allocate_memory* AllocateMemory = 0;
yd_global_variable deallocate_memory* DeallocateMemory = 0;

//
// NOTE(yuval): Public API Function Declarations
//

internal void* Copy(void* DestInit, const void* SourceInit, size_t Size);

#if !defined(CopyArray)
# define CopyArray(Dest, Source, Count) Copy((Dest), (Source), (Count) * sizeof(*(Source)))
#endif

internal void* ZeroSize(void* Ptr, size_t Size);

#if !defined(ZeroStruct)
# define ZeroStruct(Instance) ZeroSize(&(Instance), sizeof(Instance))
#endif

#if !defined(ZeroArray)
# define ZeroArray(Pointer, Count) ZeroSize(Pointer, (Count) * sizeof(*(Pointer)))
#endif

inline arena_push_params DefaultArenaParams();
inline arena_push_params ArenaAlignClear(yd_u32 Alignment);
inline arena_push_params ArenaAlignNoClear(yd_u32 Alignment);
inline arena_push_params ArenaAlign(yd_u32 Alignment, yd_b32 ShouldClear);
inline arena_push_params ArenaNoClear();
inline arena_bootstrap_params DefaultBootstrapParams()
inline arena_bootstrap_params NonRestoredArena();

inline size_t GetAlignmentOffset(memory_arena* Arena, size_t Alignment);
inline size_t GetEffectiveSizeFor(memory_arena* Arena, size_t SizeInit,
                                  arena_push_params Params = DefaultArenaParams());

inline void* PushSize_(memory_arena* Arena, size_t SizeInit,
                       arena_push_params Params = DefaultArenaParams())

#if !defined(PushSize)
# define PushSize(Arena, Size, ...) PushSize_(Arena, Size, ## __VA_ARGS__)
#endif

#if !defined(PushStruct)
# define PushStruct(Arena, Type, ...) (Type*)PushSize_(Arena, sizeof(Type), ## __VA_ARGS__)
#endif

#if !defined(PushArray)
# define PushArray(Arena, Type, Count, ...) (Type*)PushSize_(Arena, (Count) * sizeof(Type), \
## __VA_ARGS__)
#endif

#if !defined(PushCopy)
# define PushCopy(Arena, Size, Source, ...) Copy(PushSize_(Arena, Size, ## __VA_ARGS__), \
(Source), Size)
#endif

inline temporary_memory BeginTemporaryMemory(memory_arena* Arena);
inline void EndTemporaryMemory(temporary_memory TempMem);
inline void CheckArena(memory_arena* Arena);

inline void Clear(memory_arena* Arena);

inline size_t GetArenaSizeRemaining(memory_arena* Arena,
                                    arena_push_params Params = DefaultArenaParams());
inline yd_b32 ArenaHasRoomFor(memory_arena* Arena, size_t SizeInit,
                              arena_push_params Params = DefaultArenaParams());

#if defined(YD_MEMORY_IMPLEMENTATION)

//
// NOTE(yuval): Memory Utility Functions
//

yd_internal void*
Copy(void* DestInit, const void* SourceInit, size_t Size)
{
    void* Result = 0;
    
    if (DestInit && SourceInit)
    {
        const yd_u8* Source = (const yd_u8*)SourceInit;
        yd_u8* Dest = (yd_u8*)DestInit;
        
        while (Size--)
        {
            *Dest++ = *Source++;
        }
        
        Result = DestInit;
    }
    
    return Result;
}

yd_internal void*
ZeroSize(void* Ptr, size_t Size)
{
    yd_u8* Byte = (yd_u8*)Ptr;
    
    while (Size--)
    {
        *Byte++ = 0;
    }
}

//
// NOTE(yuval): Arena & Memory Parameters
//

inline arena_push_params
DefaultArenaParams()
{
    arena_push_params Params;
    Params.Flags = ArenaFlag_ClearToZero;
    Params.Alignment = 4;
    
    return Params;
}

inline arena_push_params
ArenaAlignClear(yd_u32 Alignment)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags |= ArenaFlag_ClearToZero;
    Params.Alignment = Alignment;
    
    return Params;
}

inline arena_push_params
ArenaAlignNoClear(yd_u32 Alignment)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    Params.Alignment = Alignment;
    
    return Params;
}

inline arena_push_params
ArenaAlign(yd_u32 Alignment, yd_b32 ShouldClear)
{
    arena_push_params Params;
    
    if (ShouldClear)
    {
        Params = ArenaAlignClear(Alignment);
    }
    else
    {
        Params = ArenaAlignNoClear(Alignment);
    }
    
    return Params;
}

inline arena_push_params
ArenaNoClear()
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    
    return Params.Flags;
}

inline arena_bootstrap_params
DefaultBootstrapParams()
{
    arena_bootstrap_params Params = {};
    return Params;
};

inline arena_bootstrap_params
NonRestoredArena()
{
    arena_bootstrap_params Params = DefaultBootstrapParams();
    Params.AllocationFlags = MemoryBlockFlag_NotRestored;
    
    return Params;
}

//
// NOTE(yuval): Arena Alignment
//

inline size_t
GetAlignmentOffset(memory_arena* Arena, size_t Alignment)
{
    size_t AlignmentOffset = 0;
    size_t TailPointer = (size_t)Arena->Base + Arena->Used;
    size_t AlignmentMask = Alignment - 1;
    
    if (TailPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (TailPointer & AlignmentMask);
    }
    
    return AlignmentOffset;
}

inline size_t
GetEffectiveSizeFor(memory_arena* Arena, size_t SizeInit, arena_push_params Params)
{
    size_t AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    size_t Size = SizeInit + AlignmentOffset;
    YDAssert(Size >= SizeInit);
    
    return Size;
}

// NOTE(yuval): Arena Push

inline yd_b32
YDMemoryIsPow2(yd_u32 Value)
{
    yd_b32 Result = ((Value & ~(Value - 1)) == Value);
    return Result;
}

inline void*
PushSize_(memory_arena* Arena, size_t SizeInit, arena_push_params Params)
{
    Assert(Params.Alignment <= 128);
    Assert(YDMemoryIsPow2(Params.Alignment));
    
    size_t Size = 0;
    if (Arena->CurrentBlock)
    {
        Size = GetEffectiveSizeFor(Arena, SizeInit, Params);
    }
    
    if (!Arena->CurrentBlock ||
        ((Arena->CurrentBlock->Used + Size) > Arena->CurrentBlock->Size))
    {
        Size = SizeInit; // NOTE(yuval): The base will automatically be aligned now!
        
        if (Arena->AllocationFlags & (MemoryBlockFlag_OverflowCheck |
                                      MemoryBlockFlag_UnderflowCheck))
        {
            Arena->MinimumBlockSize = 0;
            Size = YDMemoryAlignPow2(Size, Params.Alignment);
        }
        else if (Arena->MinimumBlockSize == 0)
        {
            // TODO(yuval): Tune default block size
            Arena->MinimumBlockSize = 1024 * 1024;
        }
        
        size_t BlockSize = YDMaximum(Size, Arena->MinimumBlockSize);
        
        memory_block* NewBlock = AllocateMemory(BlockSize, Arena->AllocationFlags);
        NewBlock->Prev = Arena->CurrentBlock;
        Arena->CurrentBlock = NewBlock;
    }
    
    YDAssert((Arena->CurrentBlock->Used + Size) <= Arena->CurrentBlock->Size);
    
    size_t AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    void* Result = Arena->CurrentBlock->Base + Arena->CurrentBlock->Used + AlignmentOffset;
    Arena->CurrentBlock->Used += Size;
    
    // NOTE(yuval): This is just to guarantee that nobody passed in an alignment
    // on their first allocation that was _greater_ than the page alignment
    Assert(Arena->CurrentBlock->Used <= Arena->CurrentBlock->Size);
    
    if (Params.Flags & ArenaFlag_ClearToZero)
    {
        ZeroSize(Result, SizeInit);
    }
    
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
    
    YDAssert(Arena->Used >= TempMem.Used);
    Arena->Used = TempMem.Used;
    
    YDAssert(Arena->TempCount > 0);
    --Arena->TempCount;
}

inline void
CheckArena(memory_arena* Arena)
{
    YDAssert(Arena->TempCount == 0);
}

//
// NOTE(yuval): Arena Size
//

inline void
GetArenaSizeRemaining(memory_arena* Arena, arena_push_params Params)
{
    size_t Result = Arena->Size - (Arena->Used + GetAlignmentOffset(Arena, Params.Alignment));
    return Result;
}

inline yd_b32
ArenaHasRoomFor(memory_arena* Arena, size_t SizeInit, arena_push_params Params)
{
    size_t Size = GetEffectiveSizeFor(Arena, SizeInit, Params);
    yd_b32 Result = ((Arena->Used + Size) <= Arena->Size);
    
    return Result;
}

#endif

#define YD_MEMORY
#endif
