#if !defined(BUILD_H)

#define DoJoin2(Arg1, Arg2) Arg1 ## Arg2
#define Join2(Arg1, Arg2) DoJoin2(Arg1, Arg2)
#define Join3(Arg1, Arg2, Arg3) Join2(Join2(Arg1, Arg2), Arg3)
#define Join4(Arg1, Arg2, Arg3, Arg4) Join2(Join3(Arg1, Arg2, Arg3), Arg4)

#include "build_api.h"

#include "build_types.h"
#include "build_for.h"
#include "build_assert.h"

#if 0
#include "build_shared.h"
#include "build_memory.h"
#include "build_intrinsics.h"
#include "build_tokenizer.h"
#include "build_parser.h"
#include "build_errors.h"
#endif // #if 0

#define BUILD_H
#endif // #if !defined(BUILD_H)