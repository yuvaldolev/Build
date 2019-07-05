#if !defined(BUILD_H)

#define BuildDoJoin2(Arg1, Arg2) Arg1 ## Arg2
#define BuildJoin2(Arg1, Arg2) BuildDoJoin2(Arg1, Arg2)
#define BuildJoin3(Arg1, Arg2, Arg3) BuildJoin2(BuildJoin2(Arg1, Arg2), Arg3)
#define BuildJoin4(Arg1, Arg2, Arg3, Arg4) BuildJoin2(BuildJoin3(Arg1, Arg2, Arg3), Arg4)

#include "build_types.h"
#include "build_for.h"
#include "build_assert.h"
#include "build_shared.h"
#include "build_memory.h"
#include "build_intrinsics.h"
#include "build_tokenizer.h"
#include "build_parser.h"
#include "build_errors.h"

#define BUILD_H
#endif