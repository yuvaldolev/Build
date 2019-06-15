#if !defined(META_TOOL_H)

#define MetaDoJoin2(Arg1, Arg2) Arg1 ## Arg2
#define MetaJoin2(Arg1, Arg2) MetaDoJoin2(Arg1, Arg2)
#define MetaJoin3(Arg1, Arg2, Arg3) MetaJoin2(MetaJoin2(Arg1, Arg2), Arg3)
#define MetaJoin4(Arg1, Arg2, Arg3, Arg4) MetaJoin2(MetaJoin3(Arg1, Arg2, Arg3), Arg4)

#include "meta_tool_types.h"
#include "meta_tool_for.h"
#include "meta_tool_assert.h"
#include "meta_tool_shared.h"
#include "meta_tool_memory.h"
#include "meta_tool_intrinsics.h"
#include "meta_tool_tokenizer.h"
#include "meta_tool_parser.h"
#include "meta_tool_errors.h"

#define META_TOOL_H
#endif