/*
 * IVTEn.cpp
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




void IVTEntry::signal() {
	lock();
	if(myEvent){
		myEvent->signal();
	}
	unlock();
}

IVTEntry::IVTEntry(int num,  routinepointer f) {
#ifndef BCC_BLOCK_IGNORE
	lock();
	myEvent = 0;
	FRoutine = f;
	numEntry = num;
	OGRoutine = getvect(numEntry);
	System::AllEntries[numEntry] = this;
	unlock();
#endif
}



