/*
 * Event.cpp
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
#include <dos.h>

Event::Event(IVTNo ivtNo){
	myImpl = new KernelEv((PCB*)System::running, ivtNo);
}

Event::~Event() {
  delete myImpl;
}

void Event::wait() {
  myImpl->wait();
}

void Event::signal() {
  myImpl->signal();
}
