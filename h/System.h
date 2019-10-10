#ifndef _system_h_
#define _system_h_
#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "Semaphor.h"
#include "IVTEn.h"
#include <dos.h>

#include "Schedule.h"


#define lock() {asm pushf; asm cli;}
#define unlock() {asm popf;}

typedef int ID;
typedef unsigned SignalId;

class Queue;
class Timer;
class ThreadsList;
class SemaphoreList;

class System{
public:
    static void initialize();
    static void terminate();
    static void block();
    static void shutdown();
    static void dispatch();

    static void putInScheduler(PCB* thread);
    static PCB* getFromScheduler();


    friend class Thread;
    friend class PCB;
    friend class Timer;

    static PCB* main_thread;
    static PCB* idle;
    static PCB* running;
    static Timer* timer;
    static int running_threads;

    static IVTEntry* AllEntries[256];

    static int signalsBlockedGlobally[16];

    static volatile ThreadsList* linker;
    static ThreadsList* allThreads;
    static SemaphoreList* allSemaphores;

    static Thread* getThreadById(ID id);
    static ID getRunningId();

    //DRUGI ZAD
    static int stopProcess; //flag vezan za handler0
    static void signalHandler0();
    static void blockSignalGlobally(SignalId signal);
    static void unblockSignalGlobally(SignalId signal);
};

#endif
