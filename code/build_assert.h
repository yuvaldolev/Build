#if !defined(BUILD_ASSERT_H)

#if BUILD_SLOW
// TODO(yuval): Temporary
#include <stdio.h>

#if 0
#define ASSERT_(expression, should_crash) \
if (!(expression)) { \
    char message[256]; \
    snprintf(message, sizeof(message), "Assertion Failure: %s at %s:%s:%d", \
    #expression, __FILE__, __FUNCTION__, __LINE__); \
    platform.display_message_box("ASSERSION FAILURE", message); \
    if (should_crash) { \
        *(volatile int*)0 = 0; \
    } \
}
#else // TODO(yuval): Temporary! Use the above macro which calls the platform for display message boxes
#define ASSERT_(expression, should_crash) \
if (!(expression)) { \
    char message[256]; \
    snprintf(message, sizeof(message), "Assertion Failure: %s at %s:%s:%d", \
    #expression, __FILE__, __FUNCTION__, __LINE__); \
    NSAlert *alert = [[[NSAlert alloc] init] autorelease]; \
    NSString* message_string = [NSString stringWithUTF8String:message]; \
    [alert setMessageText:@"ASSERSION FAILURE"]; \
    [alert setInformativeText:message_string]; \
    [alert runModal]; \
    if (should_crash) { \
        *(volatile int*)0 = 0; \
    } \
}
#endif // #if 0

#define ASSERT(expression) ASSERT_((expression), true)
#define SOFT_ASSERT(expression) ASSERT_((expression), false)

#else

#define ASSERT(expression)
#define SOFT_ASSERT(expression)
#endif // #if BUILD_SLOW

#define BUILD_ASSERT_H
#endif // #if !defined(BUILD_ASSERT_H)