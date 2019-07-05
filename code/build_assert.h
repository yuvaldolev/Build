#if !defined(BUILD_ASSERT_H)

#if BUILD_SLOW
// TODO(yuval): Temporary
#include <stdio.h>

#if 0
#define Assert_(Expression, ShouldCrash) \
if (!(Expression)) \
{ \
    char Message[256]; \
    snprintf(Message, sizeof(Message), "Assertion Failure: %s at %s:%s:%d", \
    #Expression, __FILE__, __FUNCTION__, __LINE__); \
    Platform.DisplayMessageBox("ASSERSION FAILURE", Message); \
    if (ShouldCrash) \
    { \
        *(volatile int*)0 = 0; \
    } \
}
#else // TODO(yuval): Temporary! Use the above macro which calls the platform for display message boxes
#define Assert_(Expression, ShouldCrash) \
if (!(Expression)) \
{ \
    char Message[256]; \
    snprintf(Message, sizeof(Message), "Assertion Failure: %s at %s:%s:%d", \
    #Expression, __FILE__, __FUNCTION__, __LINE__); \
    NSAlert *Alert = [[[NSAlert alloc] init] autorelease]; \
    NSString* MessageString = [NSString stringWithUTF8String:Message]; \
    [Alert setMessageText:@"ASSERSION FAILURE"]; \
    [Alert setInformativeText:MessageString]; \
    [Alert runModal]; \
    if (ShouldCrash) \
    { \
        *(volatile int*)0 = 0; \
    } \
}
#endif

#define Assert(Expression) Assert_((Expression), true)
#define SoftAssert(Expression) Assert_((Expression), false)

#else
#define Assert(Expression)
#define SoftAssert(Expression)
#endif

#define BUILD_ASSERT_H
#endif