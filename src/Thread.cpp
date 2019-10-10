#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "Semaphor.h"
#include <dos.h>


Thread::Thread (StackSize defaultStackSize, Time defaultTimeSlice) :
myPCB(new PCB(this, defaultTimeSlice, defaultStackSize)) {}

void Thread::start(){
	myPCB->start();
}

Thread:: ~Thread() {
	delete myPCB;
}

void Thread::waitToComplete() {
	myPCB->waitToComplete();
}

ID Thread::getRunningId() {
	return System::getRunningId();
}

Thread* Thread::getThreadById(ID id){
	return System::getThreadById(id);
}

ID Thread::getId(){
	return myPCB->getId();
}

void dispatch()
{
	System::dispatch();
}

//DRUGI ZAD
void Thread::signal(SignalId signal) {
	myPCB->signal(signal);
}
void Thread::registerHandler(SignalId signal, SignalHandler handler) {
	myPCB->registerHandler(signal, handler);
}

void Thread::unregisterAllHandlers(SignalId id) {
	myPCB->unregisterAllHandlers(id);
}
void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2) {
	myPCB->swap(id, hand1, hand2);
}
void Thread::blockSignal(SignalId signal) {
	myPCB->blockSignal(signal);
}
void Thread::blockSignalGlobally(SignalId signal) {
	System::blockSignalGlobally(signal);
}
void Thread::unblockSignal(SignalId signal) {
	myPCB->unblockSignal(signal);
}
void Thread::unblockSignalGlobally(SignalId signal) {
	System::unblockSignalGlobally(signal);
}
