/*
 * KerEv.cpp
 *
 *  Created on: Aug 15, 2019
 *      Author: OS1
 */

#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "Event.h"
#include "KerEv.h"
#include "IVTEn.h"
#include <dos.h>




void KernelEv::override() {
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtno, (*System::AllEntries[ivtno]).getFRoutine());
#endif
}

void KernelEv::restore() {
#ifndef BCC_BLOCK_IGNORE
	setvect(ivtno, (*System::AllEntries[ivtno]).getOGRoutine());
#endif
}


KernelEv::KernelEv(PCB* mine, unsigned char no) {
	lock();
	myThread = mine;
	ivtno = no;
	val = 1;
	System::AllEntries[ivtno]->myEvent = this;
	override();
	unlock();
}

KernelEv::~KernelEv() {
	lock();
	restore();
	unlock();
}

void KernelEv::wait() {
	lock();
		if ((System::running == myThread) && (val == 1)) {
			val = 0;
			System::block();
		}
	unlock();
}

void KernelEv::signal() {
		lock();
		if(val == 0 ){
			myThread->state = READY;
			System::putInScheduler(myThread);
			val = 1;
		}
		unlock();
}


