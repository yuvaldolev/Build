#if !defined(BUILD_PROFILE_H)

enum time_event_type
{
    TimeEvent_None,
    
    TimeEvent_BeginBlock,
    TimeEvent_EndBlock
};

struct time_event
{
    time_event_type Type;
    const char* Name;
    // TODO(yuval): Maybe add a name hash so that we would be able to quickly search an event by its name
    
    u64 Clock;
    u64 CycleCount;
    
    // TODO(yuval): Change this to a union as more time_event types are added
    time_event* Parent; // NOTE(yuval): Used to TimeEvent_BeginBlock
    
};

struct time_events_queue
{
    time_event Events[1024];
    umm ReadIndex;
    umm WriteIndex;
};

// TODO(yuval): Maybe force sending CycleCount (get rid of the default value for the parameter)
inline b32
RecordTimeEvent_(time_events_queue* Queue, time_event_type Type,
                 const char* Name, u64 Clock, u64 CycleCount = 0)
{
    umm NextWriteIndex = (Queue->WriteIndex + 1) % ArrayCount(Queue->Events);
    b32 CanRecordTimeEvent = (NextWriteIndex != Queue->ReadIndex);
    
    // TODO(yuval): Make this write thread-safe
    if (CanRecordTimeEvent)
    {
        time_event* Event = &Queue->Events[Queue->WriteIndex];
        Queue->WriteIndex = NextWriteIndex;
        
        Event->Type = Type;
        Event->Name = Name;
        Event->Clock = Clock;
        Event->CycleCount = CycleCount;
    }
    
    return CanRecordTimeEvent;
}

#define RecordTimeEvent(Queue, Type, Name) RecordTimeEvent_(Queue, Type, Name, \
Platform.GetWallClock(), \
__rdtsc())
inline time_event*
ReadTimeEvent(time_events_queue* Queue)
{
    // TODO(yuval): Make this read thread-safe
    time_event* Event = 0;
    
    if (Queue->ReadIndex != Queue->WriteIndex)
    {
        Event = &Queue->Events[Queue->ReadIndex++];
        Queue->ReadIndex %= ArrayCount(Queue->Events);
    }
    
    return Event;
}

// TODO(yuval): Maybe take a queue parameter?
#define BeginTimedBlock(Queue, Name) { RecordTimeEvent(Queue, TimeEvent_BeginBlock, Name); }
#define EndTimedBlock(Queue) { RecordTimeEvent(Queue, TimeEvent_EndBlock, "EndBlock_"); }

struct timed_block
{
    time_events_queue* Queue;
    
    timed_block(const char* Name, time_events_queue* Queue_)
    {
        Queue = Queue_;
        BeginTimedBlock(Queue, Name);
    }
    
    ~timed_block()
    {
        EndTimedBlock(Queue);
    }
};

#define TimedBlock(Name, Queue) timed_block Join2(TimedBlock_, __COUNTER__)(Name)
#define TimedFunction(Name) TimedBlock(__FUNCTION__, Queue)

#define BUILD_PROFILE_H
#endif // #if !defined(BUILD_PROFILE_H)