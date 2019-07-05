#if !defined(BUILD_MEMORY_H)

// TODO(yuval): Replace all of these with my own memory management using memory arena
#define Alloc(Type) (Type*)Alloc_(sizeof(Type))
#define AllocArray(Type, Count) (Type*)Alloc_((Count) * sizeof(Type))

internal void*
Alloc_(memory_index Size)
{
    void* Result = malloc(Size);
    
    if (Result)
    {
        ZeroSize(Result, Size);
    }
    
    return Result;
}

#define BUILD_MEMORY_H
#endif