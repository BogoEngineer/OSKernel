#ifndef _pcb_h_
#define _pcb_h_
#include "Thread.h"
#include "Lists.h"
#include "System.h"
#include <dos.h>
#define MAX_SIGNALS 16

typedef int (*function_pointer)();
typedef void (*function_pointer_void)();
typedef int ID;
typedef unsigned int Time;
typedef unsigned SignalId;
typedef void (*SignalHandler)();

class ThreadsList;
class Thread;
class SignalList;
class HandlerList;


enum State{
	        NEW,
	        READY,
	        BLOCKED,
	        FINISHED
	    };

class PCB{
public:
    PCB(Thread* t, unsigned int ts, unsigned long stackSize);
    PCB(function_pointer_void fun);//za idle PCB
    PCB(); //za main PCB
    ~PCB();

    volatile State state;
    volatile unsigned *stack; //netestirano, stek za nit
    volatile unsigned sp;
	volatile unsigned ss;
	volatile unsigned bp;
    volatile static ID lastID;
	volatile ID _ID;
    volatile unsigned int time_slice;
    volatile Thread* myThread;
    volatile ThreadsList* waitingOnMeQ;

    volatile int waitingFlag;
    volatile Time blockedLeft;

    //DRUGI ZAD
    volatile PCB* myParent;
    volatile int signalsBlockedLocally[MAX_SIGNALS];
    volatile SignalList* pendingSignals;
    volatile HandlerList* allHandlers[MAX_SIGNALS];

    void copyFromParent();

    void createStack(void (*body)(), unsigned long stackSize);

    void waitToComplete();
    void start();
    void setTimeSlice(unsigned int t);
    ID getId();
    static void wrapper();
    static void killThread(PCB* thread);


    //DRUGI ZAD
    void signal(SignalId signal);
    void registerHandler(SignalId signal, SignalHandler handler);
    void unregisterAllHandlers(SignalId id);
    void swap(SignalId id, SignalHandler hand1, SignalHandler hand2);
    void blockSignal(SignalId signal);
    void unblockSignal(SignalId signal);
};

#endif
