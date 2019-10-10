/*
 * KerEv.h
 *
 *  Created on: Aug 15, 2019
 *      Author: OS1
 */

#ifndef KEREV_H_
#define KEREV_H_

#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "Event.h"
#include "IVTEn.h"
#include <dos.h>


typedef void interrupt(*routinepointer)(...);

class KernelEv {
private:
	unsigned char ivtno;
	PCB *myThread;
	int val;
public:
	friend class IVTEntry;
	void wait();
	void signal();
	void override();
	void restore();
	KernelEv(PCB* mine, unsigned char no);
	~KernelEv();
};





#endif /* KEREV_H_ */
