#ifndef _Semaphor_h_
#define _Semaphor_h_
#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include <dos.h>

typedef unsigned int Time;

class KernelSem;
class Semaphore {
public:
Semaphore (int init=1);
virtual ~Semaphore ();
virtual int wait (Time maxTimeToWait);
virtual int signal(int n = 0);
int val () const; // Returns the current value of the semaphore

private:
KernelSem* myImpl;
};
#endif
