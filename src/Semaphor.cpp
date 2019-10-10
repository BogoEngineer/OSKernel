#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include <dos.h>


Semaphore:: Semaphore(int init) {
	lock();
	myImpl = new KernelSem(init);
	unlock();
}

int Semaphore::signal(int n) {
	return Semaphore:: myImpl->signal(n);
}

int Semaphore::wait (Time maxTimeToWait) {
	return Semaphore:: myImpl->wait(maxTimeToWait);
}

int Semaphore::val() const {
	return myImpl->val();
}

Semaphore:: ~Semaphore() {
	lock();
	delete  myImpl;
	unlock();
}
