#if !defined(META_TOOL_FOREACH_H)

#define ArrayForeach(val, array) \
for (int i = 0; i < ArrayCount(array); ++i) \
for (bool b = true; b; b = false) \
for (val = (array)[i]; b; b = false)

#define ArrayForeachRef(val, array) \
for (int i = 0; i < ArrayCount(array); ++i) \
for (bool b = true; b; b = false) \
for (val = &(array)[i]; b; b = false)

#define META_TOOL_FOREACH_H
#endif