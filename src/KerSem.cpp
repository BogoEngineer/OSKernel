/*
 * KerSem.cpp
 *
 *  Created on: Aug 13, 2019
 *      Author: OS1
 */

#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "Semaphor.h"
#include "KerSem.h"
#include <dos.h>


KernelSem::KernelSem(int init){
	lock();
	initial = value = init;
	blockedThreads = new ThreadsList();
	System::allSemaphores->add(this);
	unlock();
}

KernelSem::~KernelSem(){
	lock();
	System::allSemaphores->remove(this);
	unlock();
}

int KernelSem::wait(Time maxTimeToWait){
	lock();
	value--;
	if(value < 0){
		System::linker = blockedThreads;
		System::running->waitingFlag = 0;
		System::running->blockedLeft = maxTimeToWait;
		System::block();
	}
	if(System::running->waitingFlag == 1){
		value++;
		System::running->waitingFlag = 0;
		unlock();
		return 0;
	}
	unlock();
	return 1;
}

int KernelSem::signal(int n){
	lock();
	if(n<0){
		unlock();
		return n;
	}
	if(n == 0){
		PCB* temp = ((ThreadsList*)blockedThreads)->get();
		if(temp != 0){
			temp->state = READY;
			System::putInScheduler(temp);
		}
		value++;
		unlock();
		return 0;
	}else{ //ako je n>0
		int count = n;
		PCB* temp;
		while(count!= 0 && (((ThreadsList*)blockedThreads)->isEmpty() == 0)){
			temp = ((ThreadsList*)blockedThreads)->get();
			temp->state = READY;
			System::putInScheduler(temp);
			count--;
		}
		value += n;
		unlock();
		return (n - count);

	}

}



