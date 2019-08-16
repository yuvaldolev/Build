#if !defined(MAC_BUILD_H)

struct platform_work_queue_entry
{
    platform_work_queue_callback* Callback;
    void* Data;
};

struct platform_work_queue
{
    sem_t SemaphoreHandle;
    
    volatile u32 NextEntryToRead;
    volatile u32 NextEntryToWrite;
    
    platform_work_queue_entry Entries[256];
};

struct mac_thread_startup
{
    platform_work_queue* Queue;
};

#define MAC_BUILD_H
#endif // #if !defined(MAC_BUILD_H)