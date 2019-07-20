#if !defined(YD_MEMORY)

/* NOTE(yuval):

Custom Memory Allocation & Deallocation Functions Can Be
Specified To Be Used With This Library!!!

By Default The Library Uses A Straight Forward Memory Allocation Function
That Just Allocates A New Memory Block And Sets Its Flags.

You can specify a memory allocation function by initializing
YDAllocateMemory to your allocation function.

You can specify a memory deallocation function by initializing
YDDeallocateMemory to your deallocation function.

Both your memory allocation and deallocation functions must be declared
using the ALLOCATE_MEMORY and DEALLOCATE_MEMORY macros in order to be
compatible with the library's allocation / deallocation function pointers.
*/

#if !defined(YD_TYPES)
#include <stdint.h>
#include <stddef.h>

#define yd_function static
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
#endif // #if !defined(YD_TYPES)

//
// NOTE(yuval): Platform Definitions
//

#if !defined(YD_WIN32)
# if defined(_WIN32)
#  define YD_WIN32 1
# else
#  define YD_WIN32 0
# endif // # if defined(_WIN32)
#endif // #if !defined(YD_WIN32)

#if !defined(YD_MACOS)
# if defined(__APPLE__)
#  define YD_MACOS 1
# else
#  define YD_MACOS 0
# endif // # if defined(__APPLE__)
#endif // #if !defined(YD_MACOS)

#if !defined(YD_LINUX)
# if defined(__linux__)
#  define YD_LINUX 1
# else
#  define YD_LINUX 0
# endif // # if defined(__linux__)
#endif // #if !defined(YD_LINUX)

//
// NOTE(yuval): Platform-Specific Includes
//

#if YD_WIN32
# include <windows.h>
#elif YD_MACOS || YD_LINUX
# include <sys/mman.h>
#endif // #if YD_WIN32

//
// NOTE(yuval): Utility Macros
//

#if !defined(YDAssert)
# define YDAssert(Expression) if (!(Expression)) { *(volatile int*)0 = 0; }
#endif // #if !defined(YDAssert)

#if !defined(YDMinimum)
#define YDMinimum(A, B) (((A) < (B)) ? (A) : (B))
#endif // #if !defined(YDMinimum)

#if !defined(YDMaximum)
# define YDMaximum(A, B) (((A) > (B)) ? (A) : (B))
#endif // #if !defined(YDMaximum)

#if !defined(YDAlignPow2)
#define YDAlignPow2(Value, Alignment) ((Value + ((Alignment) - 1)) & ~((Value - Value) + (Alignment) - 1))
#endif // #if !defined(YDAlignPow2)

//
// NOTE(yuval): Type Definitions
//

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
// NOTE(yuval): Memory Block Allocation & Deallocation
//

#define ALLOCATE_MEMORY(Name) memory_block* Name(size_t Size, u64 Flags)
typedef ALLOCATE_MEMORY(allocate_memory);

#define DEALLOCATE_MEMORY(Name) void Name(memory_block* Block)
typedef DEALLOCATE_MEMORY(deallocate_memory);

extern allocate_memory* YDAllocateMemory;
extern deallocate_memory* YDDeallocateMemory;

//
// NOTE(yuval): Public API Function Declarations
//

yd_function void* Copy(void* DestInit, const void* SourceInit, size_t Size);

#if !defined(CopyArray)
# define CopyArray(Dest, Source, Count) Copy((Dest), (Source), (Count) * sizeof(*(Source)))
#endif // #if !defined(CopyArray)

yd_function void* ZeroSize(void* Ptr, size_t Size);

#if !defined(ZeroStruct)
# define ZeroStruct(Instance) ZeroSize(&(Instance), sizeof(Instance))
#endif // #if !defined(ZeroStruct)

#if !defined(ZeroArray)
# define ZeroArray(Pointer, Count) ZeroSize(Pointer, (Count) * sizeof(*(Pointer)))
#endif // #if !defined(ZeroArray)

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
#endif // #if !defined(PushSize)

#if !defined(PushStruct)
# define PushStruct(Arena, Type, ...) (Type*)PushSize_(Arena, sizeof(Type), ## __VA_ARGS__)
#endif // #if !defined(PushStruct)

#if !defined(PushArray)
# define PushArray(Arena, Type, Count, ...) (Type*)PushSize_(Arena, (Count) * sizeof(Type), \
## __VA_ARGS__)
#endif // #if !defined(PushArray)

#if !defined(PushCopy)
# define PushCopy(Arena, Size, Source, ...) Copy(PushSize_(Arena, Size, ## __VA_ARGS__), \
(Source), Size)
#endif // #if !defined(PushCopy)

inline temporary_memory BeginTemporaryMemory(memory_arena* Arena);
inline void EndTemporaryMemory(temporary_memory TempMem);
inline void CheckArena(memory_arena* Arena);

inline void Clear(memory_arena* Arena);

#define YD_MEMORY
#endif // #if !defined(YD_MEMORY)

//
// NOTE(yuval): IMPLEMENTATION
//

#if defined(YD_MEMORY_IMPLEMENTATION)

//
// NOTE(yuval): Memory Utility Functions
//

yd_function void*
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

yd_function void*
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
    YDAssert(Params.Alignment <= 128);
    YDAssert(YDMemoryIsPow2(Params.Alignment));
    
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
            Size = YDAlignPow2(Size, Params.Alignment);
        }
        else if (Arena->MinimumBlockSize == 0)
        {
            // TODO(yuval): Tune default block size
            Arena->MinimumBlockSize = 1024 * 1024;
        }
        
        size_t BlockSize = YDMaximum(Size, Arena->MinimumBlockSize);
        
        YDAssert(YDAllocateMemory);
        memory_block* NewBlock = YDAllocateMemory(BlockSize, Arena->AllocationFlags);
        NewBlock->Prev = Arena->CurrentBlock;
        Arena->CurrentBlock = NewBlock;
    }
    
    YDAssert((Arena->CurrentBlock->Used + Size) <= Arena->CurrentBlock->Size);
    
    size_t AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    void* Result = Arena->CurrentBlock->Base + Arena->CurrentBlock->Used + AlignmentOffset;
    Arena->CurrentBlock->Used += Size;
    
    // NOTE(yuval): This is just to guarantee that nobody passed in an alignment
    // on their first allocation that was _greater_ than the page alignment
    YDAssert(Arena->CurrentBlock->Used <= Arena->CurrentBlock->Size);
    
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
    Result.Block = Arena->CurrentBlock;
    Result.Used = Arena->CurrentBlock ? Arena->CurrentBlock->Used : 0;
    
    ++Arena->TempCount;
    
    return Result;
}

inline void
YDMemoryFreeLastBlock(memory_arena* Arena)
{
    memory_block* ToFree = Arena->CurrentBlock;
    Arena->CurrentBlock = ToFree->Prev;
    
    YDAssert(YDDeallocateMemory);
    YDDeallocateMemory(Free);
}

inline void
EndTemporaryMemory(temporary_memory TempMem)
{
    memory_arena* Arena = TempMem.Arena;
    
    while (Arena->CurrentBlock != TempMem.Block)
    {
        YDMemoryFreeLastBlock(TempMem);
    }
    
    if (Arena->CurrentBlock)
    {
        YDAssert(Arena->CurrentBlock->Used >= TempMem.Used);
        Arena->CurrentBlock->Used = TempMem.Used;
    }
    
    YDAssert(Arena->TempCount > 0);
    --Arena->TempCount;
}

inline void
CheckArena(memory_arena* Arena)
{
    YDAssert(Arena->TempCount == 0);
}

//
// NOTE(yuval): Arena Clearing
//

inline void
Clear(memory_arena* Arena)
{
    for (;;)
    {
        b32 IsLastBlock = (Arena->CurrentBlock->Prev == 0);
        YDMemoryFreeLastBlock(Arena);
        
        if (IsLastBlock)
        {
            break;
        }
    }
}


ALLOCATE_MEMORY(YDAllocateMemory_)
{
#if 0
    // NOTE(yuval): We require the memory block headers not to change the cache
    // line alignment of an allocation
    YDAssert(sizeof(memory_block) == 64);
#endif // #if 0
    
    size_t PageSize = 4096;
    size_t TotalSize = Size + sizeof(memory_block);
    size_t BaseOffset = sizeof(memory_block);
    size_t ProtectionOffset = 0;
    
    if (Flags & MemoryBlockFlag_UnderflowCheck)
    {
        TotalSize = Size + 2 * PageSize;
        BaseOffset = 2 * PageSize;
        ProtectionOffset = PageSize;
    }
    else if (Flags & MemoryBlockFlag_OverflowCheck)
    {
        size_t SizeRoundedUp = AlignPow2(Size, PageSize);
        TotalSize = SizeRoundedUp + 2 * PageSize;
        BaseOffset = PageSize + SizeRoundedUp - Size;
        ProtectionOffset = PageSize + SizeRoundedUp;
    }
    
    memory_block* Block = 0;
    
#if YD_WIN32
    
#elif YD_MACOS || YD_LINUX
    b
#endif // #if YD_WIN32
}

DEALLOCATE_MEMORY(YDDeallocateMemory_)
{
    
}

allocate_memory* YDAllocateMemory = YDAllocateMemory_;
deallocate_memory* YDDeallocateMemory = YDDeallocateMemory_;

#endif // #if defined(YD_MEMORY_IMPLEMENTATION)
