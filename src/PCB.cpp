
#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include <dos.h>



#define MAX_STACK_SIZE 32768

volatile ID PCB::lastID = 0;

void PCB::createStack(function_pointer_void fun, unsigned long stackSize){
    if(stackSize > MAX_STACK_SIZE) stackSize = MAX_STACK_SIZE;
    stackSize /= sizeof(long unsigned); //long ili obican
    stack = new unsigned[stackSize];
    stack[stackSize - 1] = 0x200;
#ifndef BCC_BLOCK_IGNORE
    stack[stackSize - 2] = FP_SEG(fun);
    stack[stackSize - 3] = FP_OFF(fun);
    sp = FP_OFF(stack + stackSize - 12);
	ss = FP_SEG(stack + stackSize - 12);
	bp = FP_OFF(stack + stackSize - 12);
#endif
}


void PCB::wrapper(){
    ((Thread*)((PCB*)(System::running))->myThread)->run();
    killThread(System::running);
}

PCB::PCB(Thread* t, unsigned int ts, unsigned long stackSize){
    lock();
    state = NEW;
    time_slice = ts;
    myThread = t;
    _ID = ++lastID;
    waitingOnMeQ = new ThreadsList();

    waitingFlag = 0;
    blockedLeft = 0;

    //DRUGI ZAD
    myParent = System::running;
    char signalsBlockedLocally[] = {0};
    pendingSignals = new SignalList();
    for(int i = 0; i<MAX_SIGNALS; i++){
    	allHandlers[i] = new HandlerList();
    }
    copyFromParent();

    createStack(PCB::wrapper, stackSize);
    System::allThreads->add(this);
    unlock();
}

PCB::PCB(function_pointer_void fun){ //konstruktor za idle
	lock();
	state = READY;
	time_slice = 1;
	myThread = 0;
	waitingOnMeQ = new ThreadsList();

	 waitingFlag = 0;
	 blockedLeft = 0;

	 //DRUGI ZAD
	 myParent = System::main_thread;
	 char signalsBlockedLocally[] = {0};
	 pendingSignals = new SignalList();
	 for(int i = 0; i<MAX_SIGNALS; i++) allHandlers[i] = new HandlerList();
	 copyFromParent();

	createStack(fun, 1024);
	unlock();
}


volatile unsigned spp;
volatile unsigned sss;
volatile unsigned bpp;
PCB::PCB(){ //za main nit
	lock();
	state = READY;
	time_slice = 0;
	myThread = new Thread();
	delete myThread->myPCB;
	myThread->myPCB = this;
	waitingOnMeQ = 0; //niko nece cekati na zavrsetak main niti
	_ID = 0;

	 waitingFlag = 0;
	 blockedLeft = 0;

	 //DRUGI ZAD
	 myParent = 0;

	 pendingSignals = new SignalList();
	 for(int i = 0; i<MAX_SIGNALS; i++) {
		 allHandlers[i] = new HandlerList();
		 signalsBlockedLocally[i] = 0;
	 }
	 copyFromParent();
	unlock();
}

void PCB::copyFromParent(){
	if(myParent == 0) return;
	pendingSignals = ((SignalList*)myParent->pendingSignals)->clone();
	for(int i = 0; i <MAX_SIGNALS; i++) {
		allHandlers[i] = ((HandlerList*)myParent->allHandlers[i])->clone();
		signalsBlockedLocally[i] = myParent->signalsBlockedLocally[i];
	}
}

void PCB::waitToComplete(){
    lock();
    if(state != FINISHED && state != NEW){
        System::linker = this->waitingOnMeQ;
        System::running->state = BLOCKED;
        System::block();
    }
    unlock();
}

void PCB::killThread(PCB* thread){
    lock();
    //DRUGI ZAD
    if(thread->myParent) ((PCB*)thread->myParent)->signal(1);

    System::running_threads--;
    PCB* temp;
    while(((ThreadsList*)thread->waitingOnMeQ)->isEmpty() == 0){
        temp = ((ThreadsList*)thread->waitingOnMeQ)->get();
        if(temp!= 0){
        	temp->state = READY;
        	System::putInScheduler((PCB*)temp);
        }
    }
    thread->state = FINISHED;
    System::dispatch();
    unlock();
}

void PCB::start(){
    lock();
    if(state == NEW){
		state = READY;
		System::running_threads++;
		System::putInScheduler(this);
    }
    unlock();
}

ID PCB::getId(){
	return _ID;
}

void PCB::setTimeSlice(unsigned int t){
    lock();
    time_slice = t;
    unlock();
}

PCB::~PCB(){
	lock();
    delete waitingOnMeQ;
    for(int i = 0; i <MAX_SIGNALS; i++) delete allHandlers[i];
    delete pendingSignals;
    delete [] (unsigned*)stack; //brisi stek
    //System::allThreads->remove(this);
    unlock();
}

//DRUGI ZAD
void PCB::signal(SignalId signal){
	lock();
	SigElement* sig = new SigElement(signal);
	((SignalList*)pendingSignals)->add(sig);
	unlock();
}
void PCB::registerHandler(SignalId signal, SignalHandler handler){
	lock();
	HandlerElement* he = new HandlerElement(handler);
	((HandlerList*)allHandlers[signal])->add(he);
	unlock();
}
void PCB::unregisterAllHandlers(SignalId id){
	lock();
	delete allHandlers[id];
	allHandlers[id] = new HandlerList();
	unlock();
}
void PCB::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){
	lock();
	((HandlerList*)allHandlers[id])->swap(hand1, hand2);
	unlock();
}
void PCB::blockSignal(SignalId signal){
	lock();
	signalsBlockedLocally[signal] = 1;
	unlock();
}
void PCB::unblockSignal(SignalId signal){
	lock();
	signalsBlockedLocally[signal] = 0;
	unlock();
}
