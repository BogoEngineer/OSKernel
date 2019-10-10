/*
 * KerSem.h
 *
 *  Created on: Aug 13, 2019
 *      Author: OS1
 */

#ifndef KERSEM_H_
#define KERSEM_H_

#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "Semaphor.h"
#include <dos.h>
class KernelSem {

public:

	volatile ThreadsList *blockedThreads;
	volatile int value;
	int initial;

	KernelSem(int init);
	~KernelSem();

	int wait(Time maxTimeToWait);
	int signal(int n = 0);
	int val() {return value;}

};





#endif /* KERSEM_H_ */
