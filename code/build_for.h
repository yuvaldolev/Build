#if !defined(BUILD_FOR_H)

///////////////////////////////
//       Array Foreach       //
///////////////////////////////
#define ArrayFor(Array) \
for (u32 Index = 0, For__ShouldBreak = 0; \
Index < ArrayCount(Array) && !For__ShouldBreak; \
++Index) \
for (b32 Join2(B, __LINE__) = true; \
Join2(B, __LINE__); \
Join2(B, __LINE__) = false) \
for (auto& It = (Array)[Index]; \
Join2(B, __LINE__); \
Join2(B, __LINE__) = false)

#define ArrayBreak For__ShouldBreak = 1

#if 0
///////////////////////////////
//         For Range         //
///////////////////////////////
#define Range2_(Beg, End) for (decltype(End) Join2(I, __LINE__) = Beg; \
Join2(I, __LINE__) < End; \
++Join2(I, __LINE__))
#define Range1_(End) Range2_(0, End)
#define GetRange_(_1, _2, Name, ...) Name
#define Range(...) GetRange_(__VA_ARGS__, Range2_, Range1_)(__VA_ARGS__)

#define ClosedRange2_(Beg, End) for (decltype(End) Join2(I, __LINE__) = Beg; \
Join2(I, __LINE__) <= End; \
++Join2(I, __LINE__))
#define ClosedRange1_(End) ClosedRange2_(0, End)
#define GetClosedRange_(_1, _2, Name, ...) Name
#define ClosedRange(...) GetClosedRange_(__VA_ARGS__, ClosedRange2_, ClosedRange1_)(__VA_ARGS__)

#define ForRange_(Name, Range) Range \
for (b32 Join2(B, __LINE__) = true; \
Join2(B, __LINE__); \
Join2(B, __LINE__) = false) \
for (auto Name = Join2(I, __LINE__); \
Join2(B, __LINE__); \
Join2(B, __LINE__) = false)

///////////////////////////////
//         For Macro         //
///////////////////////////////
#define GetFor_(_1, _2, Name, ...) Name
#define For(...) GetFor_(__VA_ARGS__, ForRange_, ArrayForeach_)(__VA_ARGS__)
#endif // #if 0

#define BUILD_FOR_H
#endif // #if !defined(BUILD_FOR_H)