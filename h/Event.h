/*
 * Event.h
 *
 *  Created on: Aug 15, 2019
 *      Author: OS1
 */

#ifndef EVENT_H_
#define EVENT_H_

#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "IVTEn.h"
#include <dos.h>

typedef unsigned char IVTNo;

class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();
protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};




#endif /* EVENT_H_ */
