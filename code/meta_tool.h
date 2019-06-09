#if !defined(META_TOOL_H)

#include "meta_tool_types.h"
#include "meta_tool_shared.h"
#include "meta_tool_intrinsics.h"
#include "meta_tool_foreach.h"
#include "meta_tool_tokenizer.h"
#include "meta_tool_ast.h"
#include "meta_tool_errors.h"

void
PlatformExitWithMessage(const char* format, ...);

b32
PlatformWriteToFile(const void* memory, memory_index memorySize);

b32
PlatformWriteTokenToFile(Token* token);

void
MetaToolProcessFile(String filename, String file);

#define META_TOOL_H
#endif