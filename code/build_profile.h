#if !defined(BUILD_PROFILE_H)

enum Time_Event_Type {
    TIME_EVENT_NONE,
    
    TIME_EVENT_BEGIN_BLOCK,
    TIME_EVENT_END_BLOCK
};

struct Time_Event {
    Time_Event_Type type;
    const char* name;
    // TODO(yuval): Maybe add a name hash so that we would be able to quickly search an event by its name
    
    u64 clock;
    u64 cycle_count;
    
    // TODO(yuval): Change this to a union as more time_event types are added
    Time_Event* parent; // NOTE(yuval): Used for TIME_EVENT_BEGIN_BLOCK
    
};

struct Time_Events_Queue {
    Time_Event events[1024];
    umm read_index;
    umm write_index;
};

// TODO(yuval): Maybe force sending CycleCount (get rid of the default value for the parameter)
inline b32
record_time_event_(Time_Events_Queue* queue, Time_Event_Type type,
                   const char* name, u64 clock, u64 cycle_count = 0) {
    umm next_write_index = (queue->write_index + 1) % ARRAY_COUNT(queue->events);
    b32 can_record_time_event = (next_write_index != queue->read_index);
    
    // TODO(yuval): Make this write thread-safe
    if (can_record_time_event) {
        Time_Event* event = &queue->events[queue->write_index];
        queue->write_index = next_write_index;
        
        event->type = type;
        event->name = name;
        event->clock = clock;
        event->cycle_count = cycle_count;
    }
    
    return can_record_time_event;
}

#define RECORD_TIME_EVENT(queue, type, name) \
record_time_event_(queue, type, name, platform.get_wall_clock(), __rdtsc())

inline Time_Event*
read_time_event(Time_Events_Queue* queue) {
    // TODO(yuval): Make this read thread-safe
    Time_Event* event = 0;
    
    if (queue->read_index != queue->write_index) {
        event = &queue->events[queue->read_index++];
        queue->read_index %= ARRAY_COUNT(queue->events);
    }
    
    return event;
}

// TODO(yuval): Maybe allow using a default queue?
#define BEGIN_TIMED_BLOCK(queue, name) { RECORD_TIME_EVENT(queue, TIME_EVENT_BEGIN_BLOCK, name); }
#define END_TIMED_BLOCK(queue) { RECORD_TIME_EVENT(queue, TIME_EVENT_END_BLOCK, "EndBlock_"); }

struct Timed_Block {
    Time_Events_Queue* queue;
    
    Timed_Block(const char* name, Time_Events_Queue* queue_) {
        queue = queue_;
        BEGIN_TIMED_BLOCK(queue, name);
    }
    
    ~Timed_Block() {
        END_TIMED_BLOCK(queue);
    }
};

#define TIMED_BLOCK(name, queue) Timed_Block JOIN2(timed_block_, __COUNTER__)(name, queue)
#define TimedFunction(queue) Timed_Block(__FUNCTION__, queue)

#define BUILD_PROFILE_H
#endif // #if !defined(BUILD_PROFILE_H)