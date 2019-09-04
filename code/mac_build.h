#if !defined(MAC_BUILD_H)

struct Platform_Work_Queue_Entry {
    Platform_Work_Queue_Callback* callback;
    void* data;
};

struct Platform_Work_Queue {
    sem_t semaphore_handle;
    
    Platform_Work_Queue_Entry entries[256];
    volatile u32 next_entry_to_read;
    volatile u32 next_entry_to_write;
    
    volatile u32 completion_count;
    volatile u32 completion_goal;
};

struct Mac_Thread_Startup {
    Platform_Work_Queue* queue;
};

#define MAC_BUILD_H
#endif // #if !defined(MAC_BUILD_H)