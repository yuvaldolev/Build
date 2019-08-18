#if !defined(YD_MEMORY)

/* NOTE(yuval):

Custom Memory Allocation & Deallocation Functions Can Be
Specified To Be Used With This Library!!!

By Default The Library Uses A Straightforward Memory Allocation Function
That Just Allocates A New Memory Block And Sets Its Flags.

You can specify a memory allocation function by initializing
YDAllocateMemory to your allocation function.

You can specify a memory deallocation function by initializing
YDDeallocateMemory to your deallocation function.

Both your memory allocation and deallocation functions must be declared
using the ALLOCATE_MEMORY and DEALLOCATE_MEMORY macros in order to be
compatible with the library.
*/

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
# define YDAlignPow2(Value, Alignment) ((Value + ((Alignment) - 1)) & \
~((Value - Value) + (Alignment) - 1))
#endif // #if !defined(YDAlignPow2)

#if !defined(YDOffsetOf)
# define YDOffsetOf(Type, Member) (umm)&(((Type*)0)->Member)
#endif // #if !defined(YDOffsetOf)

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
    yd_umm Size;
    yd_umm Used;
    
    yd_umm TotalAllocatedSize;
    
    memory_block* Prev;
};

struct memory_arena
{
    memory_block* CurrentBlock;
    yd_umm MinimumBlockSize;
    
    yd_u64 AllocationFlags;
    yd_s32 TempCount;
};

struct temporary_memory
{
    memory_arena* Arena;
    memory_block* Block;
    yd_umm Used;
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
    yd_umm MinimumBlockSize;
};

//
// NOTE(yuval): Memory Block Allocation & Deallocation Function Types
//

#define ALLOCATE_MEMORY(Name) memory_block* Name(yd_umm Size, yd_u64 Flags)
typedef ALLOCATE_MEMORY(allocate_memory);

#define DEALLOCATE_MEMORY(Name) void Name(memory_block* Block)
typedef DEALLOCATE_MEMORY(deallocate_memory);

extern allocate_memory* YDAllocateMemory;
extern deallocate_memory* YDDeallocateMemory;

//
// NOTE(yuval): Public API Function Declarations
//

void* Copy(void* DestInit, const void* SourceInit, yd_umm Size);

#if !defined(CopyArray)
# define CopyArray(Dest, Source, Count) Copy((Dest), (Source), (Count) * sizeof(*(Source)))
#endif // #if !defined(CopyArray)

void ZeroSize(void* Ptr, yd_umm Size);

#if !defined(ZeroStruct)
# define ZeroStruct(Instance) ZeroSize(&(Instance), sizeof(Instance))
#endif // #if !defined(ZeroStruct)

#if !defined(ZeroArray)
# define ZeroArray(Pointer, Count) ZeroSize(Pointer, (Count) * sizeof(*(Pointer)))
#endif // #if !defined(ZeroArray)

// NOTE(yuval): This function declaration is an inline function and it's declared here only
// because PushSize_'s declaration uses it
yd_internal inline arena_push_params DefaultArenaParams();

void* PushSize_(memory_arena* Arena, yd_umm SizeInit,
                arena_push_params Params = DefaultArenaParams());

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
# define PushCopy(Arena, Source, Size, ...) Copy(PushSize_(Arena, Size, ## __VA_ARGS__), \
(Source), Size)
#endif // #if !defined(PushCopy)

void Clear(memory_arena* Arena);

//
// NOTE(yuval): Public API Inline Functions
//

//
// NOTE(yuval): Arena & Memory Parameters
//

yd_internal inline arena_push_params
DefaultArenaParams()
{
    arena_push_params Params;
    Params.Flags = ArenaFlag_ClearToZero;
    Params.Alignment = 4;
    
    return Params;
}

yd_internal inline arena_push_params
ArenaAlignClear(yd_u32 Alignment)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags |= ArenaFlag_ClearToZero;
    Params.Alignment = Alignment;
    
    return Params;
}

yd_internal inline arena_push_params
ArenaAlignNoClear(yd_u32 Alignment)
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    Params.Alignment = Alignment;
    
    return Params;
}

yd_internal inline arena_push_params
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

yd_internal inline arena_push_params
ArenaNoClear()
{
    arena_push_params Params = DefaultArenaParams();
    Params.Flags &= ~ArenaFlag_ClearToZero;
    
    return Params;
}

yd_internal inline arena_bootstrap_params
DefaultBootstrapParams()
{
    arena_bootstrap_params Params = {};
    return Params;
};

yd_internal inline arena_bootstrap_params
NonRestoredArena()
{
    arena_bootstrap_params Params = DefaultBootstrapParams();
    Params.AllocationFlags = MemoryBlockFlag_NotRestored;
    
    return Params;
}

//
// NOTE(yuval): Arena Alignment
//

yd_internal inline yd_umm
GetAlignmentOffset(memory_arena* Arena, yd_umm Alignment)
{
    yd_umm AlignmentOffset = 0;
    yd_umm TailPointer = (yd_umm)Arena->CurrentBlock->Base + Arena->CurrentBlock->Used;
    yd_umm AlignmentMask = Alignment - 1;
    
    if (TailPointer & AlignmentMask)
    {
        AlignmentOffset = Alignment - (TailPointer & AlignmentMask);
    }
    
    return AlignmentOffset;
}

yd_internal inline yd_umm
GetEffectiveSizeFor(memory_arena* Arena, yd_umm SizeInit, arena_push_params Params)
{
    yd_umm AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
    yd_umm Size = SizeInit + AlignmentOffset;
    YDAssert(Size >= SizeInit);
    
    return Size;
}

//
// NOTE(yuval): Temporary Arena Memory
//

yd_internal inline temporary_memory
BeginTemporaryMemory(memory_arena* Arena)
{
    temporary_memory Result;
    
    Result.Arena = Arena;
    Result.Block = Arena->CurrentBlock;
    Result.Used = Arena->CurrentBlock ? Arena->CurrentBlock->Used : 0;
    
    ++Arena->TempCount;
    
    return Result;
}

yd_internal inline void
YDMemoryFreeLastBlock(memory_arena* Arena)
{
    memory_block* ToFree = Arena->CurrentBlock;
    Arena->CurrentBlock = ToFree->Prev;
    
    YDAssert(YDDeallocateMemory);
    YDDeallocateMemory(ToFree);
}

yd_internal inline void
EndTemporaryMemory(temporary_memory TempMem)
{
    memory_arena* Arena = TempMem.Arena;
    
    while (Arena->CurrentBlock != TempMem.Block)
    {
        YDMemoryFreeLastBlock(Arena);
    }
    
    if (Arena->CurrentBlock)
    {
        YDAssert(Arena->CurrentBlock->Used >= TempMem.Used);
        Arena->CurrentBlock->Used = TempMem.Used;
    }
    
    YDAssert(Arena->TempCount > 0);
    --Arena->TempCount;
}

yd_internal inline void
CheckArena(memory_arena* Arena)
{
    YDAssert(Arena->TempCount == 0);
}

//
// NOTE(yuval): Bootstrap Push
//

yd_internal inline void*
BootstrapPushSize_(yd_umm Size, yd_umm OffsetToArena,
                   arena_bootstrap_params BootstrapParams = DefaultBootstrapParams(),
                   arena_push_params PushParams = DefaultArenaParams())
{
    memory_arena Bootstrap = {};
    Bootstrap.AllocationFlags = BootstrapParams.AllocationFlags;
    Bootstrap.MinimumBlockSize = BootstrapParams.MinimumBlockSize;
    
    void* Result = PushSize(&Bootstrap, Size, PushParams);
    *(memory_arena*)((yd_u8*)Result + OffsetToArena) = Bootstrap;
    
    return Result;
}

#if !defined(BootstrapPushStruct)
# define BootstrapPushStruct(Type, Member, ...) (Type*)BootstrapPushSize_(sizeof(Type), \
YDOffsetOf(Type, Member), \
## __VA_ARGS__)
#endif // #if !defined(BootstrapPushStruct)

#define YD_MEMORY
#endif // #if !defined(YD_MEMORY)

//
// NOTE(yuval): Exported Function Implementations
//

#if defined(YD_MEMORY_IMPLEMENTATION)

//
// NOTE(yuval): Memory Allocation & Deallocation
//

ALLOCATE_MEMORY(YDAllocateMemory_)
{
#if 0
    // NOTE(yuval): We require the memory block headers not to change the cache
    // line alignment of an allocation
    YDAssert(sizeof(memory_block) == 64);
#endif // #if 0
    
    yd_umm PageSize = 4096;
    yd_umm TotalSize = Size + sizeof(memory_block);
    yd_umm BaseOffset = sizeof(memory_block);
    yd_umm ProtectOffset = 0;
    
    if (Flags & MemoryBlockFlag_UnderflowCheck)
    {
        TotalSize = Size + 2 * PageSize;
        BaseOffset = 2 * PageSize;
        ProtectOffset = PageSize;
    }
    else if (Flags & MemoryBlockFlag_OverflowCheck)
    {
        yd_umm SizeRoundedUp = YDAlignPow2(Size, PageSize);
        TotalSize = SizeRoundedUp + 2 * PageSize;
        BaseOffset = PageSize + SizeRoundedUp - Size;
        ProtectOffset = PageSize + SizeRoundedUp;
    }
    
    memory_block* Block = 0;
    
#if YD_WIN32
    Block = (memory_block*)VirtualAlloc(0, TotalSize,
                                        MEM_RESERVE | MEM_COMMIT,
                                        PAGE_READWRITE);
#elif YD_MACOS || YD_LINUX
    Block = (memory_block*)mmap(0, TotalSize,
                                PROT_READ | PROT_WRITE,
                                MAP_PRIVATE | MAP_ANONYMOUS,
                                -1, 0);
    YDAssert(Block != MAP_FAILED);
#endif // #if YD_WIN32
    
    YDAssert(Block);
    Block->Base = (yd_u8*)Block + BaseOffset;
    YDAssert(Block->Used == 0);
    YDAssert(Block->Prev == 0);
    
    if (Flags & (MemoryBlockFlag_OverflowCheck | MemoryBlockFlag_UnderflowCheck))
    {
#if YD_WIN32
        DWORD OldProtect = 0;
        BOOL IsProtected = VirtualProtect((yd_u8*)Block + ProtectOffset, PageSize,
                                          PAGE_NOACCESS, &OldProtect);
        YDAssert(IsProtected);
#elif YD_MACOS || YD_LINUX
        yd_s32 ProtectResult = mprotect((yd_u8*)Block + ProtectOffset, PageSize, PROT_NONE);
        YDAssert(ProtectResult == 0);
#endif // #if YD_WIN32
    }
    
    Block->Flags = Flags;
    Block->Size = Size;
    Block->TotalAllocatedSize = TotalSize;
    
    return Block;
}

DEALLOCATE_MEMORY(YDDeallocateMemory_)
{
#if YD_WIN32
    BOOL Result = VirtualFree(Block, 0, MEM_RELEASE);
    YDAssert(Result);
#elif YD_MACOS || YD_LINUX
    yd_s32 Result = munmap(Block, Block->TotalAllocatedSize);
    YDAssert(Result == 0);
#endif // #if YD_WIN32
}

allocate_memory* YDAllocateMemory = YDAllocateMemory_;
deallocate_memory* YDDeallocateMemory = YDDeallocateMemory_;

//
// NOTE(yuval): Memory Utility Functions
//

void*
Copy(void* DestInit, const void* SourceInit, yd_umm Size)
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

void
ZeroSize(void* Ptr, yd_umm Size)
{
    yd_u8* Byte = (yd_u8*)Ptr;
    
    while (Size--)
    {
        *Byte++ = 0;
    }
}

//
// NOTE(yuval): Arena Push
//

yd_internal inline yd_b32
YDMemoryIsPow2(yd_u32 Value)
{
    yd_b32 Result = ((Value & ~(Value - 1)) == Value);
    return Result;
}

void*
PushSize_(memory_arena* Arena, yd_umm SizeInit, arena_push_params Params)
{
    YDAssert(Params.Alignment <= 128);
    YDAssert(YDMemoryIsPow2(Params.Alignment));
    
    yd_umm Size = 0;
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
        
        yd_umm BlockSize = YDMaximum(Size, Arena->MinimumBlockSize);
        
        YDAssert(YDAllocateMemory);
        memory_block* NewBlock = YDAllocateMemory(BlockSize, Arena->AllocationFlags);
        NewBlock->Prev = Arena->CurrentBlock;
        Arena->CurrentBlock = NewBlock;
    }
    
    YDAssert((Arena->CurrentBlock->Used + Size) <= Arena->CurrentBlock->Size);
    
    yd_umm AlignmentOffset = GetAlignmentOffset(Arena, Params.Alignment);
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
// NOTE(yuval): Arena Clearing
//

void
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

#endif // #if defined(YD_MEMORY_IMPLEMENTATION)
