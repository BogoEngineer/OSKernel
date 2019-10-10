/*
 * IVTEn.h
 *
 *  Created on: Aug 15, 2019
 *      Author: OS1
 */

#ifndef IVTEN_H_
#define IVTEN_H_

#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "Event.h"
#include "KerEv.h"
#include <dos.h>

typedef void interrupt(*routinepointer)(...);


class IVTEntry {
private:
	int numEntry;
	routinepointer OGRoutine;
	routinepointer FRoutine;
	KernelEv *myEvent;
public:

	friend class KernelEv;
	void signal();
	IVTEntry(int num,  routinepointer f);
	IVTEntry();
	routinepointer getFRoutine() {
		return FRoutine;
	}
	routinepointer getOGRoutine() {
		return OGRoutine;
	}
	void callOld() {
		OGRoutine();
	}
};

#define PREPAREENTRY(numEntry,cO)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry,inter##numEntry);\
void interrupt inter##numEntry(...){\
	newEntry##numEntry.signal();\
	if(cO == 1)\
		newEntry##numEntry.callOld();\
}





#endif /* IVTEN_H_ */
