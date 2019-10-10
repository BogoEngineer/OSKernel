#ifndef _timer_h_
#define _timer_h_
#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include <dos.h>


#define NumOfTasks 3
enum Job {
    STOPWATCH,
    BLOCK,
    SHUTDOWN,
    DISPATCH
};

class Timer{
public:
    static void newTimerRoutine();
    static void dispatch();
    static void block();
    static void shutdown();
    static void stopwatch();
    Timer();
    ~Timer();

    Job job; //pokazivac u nizu koji govori sta treba da se radi u timeru
    static long long unsigned int time;
    static int thread_time;
    static char change_flag;
    void (*task[NumOfTasks])(); //niz pokazivaca na sve moguce funkcionalnosti timera
    static void call();
    static void override();
    static void restore();
};

#endif
