#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include <dos.h>


volatile int no_optimisation = 1;

void idle_run(){
	unsigned int i = 0;
	while(no_optimisation){}
}

PCB* System::running = 0;
Timer* System::timer = new Timer();
PCB* System::idle = new PCB(idle_run);
PCB* System::main_thread = new PCB();
volatile ThreadsList* System::linker = 0;
ThreadsList* System::allThreads = new ThreadsList();
SemaphoreList* System::allSemaphores = new SemaphoreList();
IVTEntry* System::AllEntries[] = {0};
int System::running_threads = 0; //pocinje od 1 jer je main nit aktivna na pocetku

int System::signalsBlockedGlobally[] = {0};
int System::stopProcess = 0;

void System::signalHandler0(){
	stopProcess = 1;
	PCB::killThread(System::running);
}

void System::initialize(){
    lock();
    //timer = new Timer();

    //main_thread = new PCB(); //napravimo novu nit koja predstavlja main nit sa podrazumevanim vrednostima za stek i timeslice
    running = main_thread;
    running->state = READY;
    
    //allThreads = new ThreadsList();
    //allSemaphores = new SemaphoreList();
    allThreads->add(main_thread);

    //idle = new PCB(idle_run);

    //DRUGI ZAD
    main_thread->registerHandler(0, signalHandler0);
    unlock();
}

void System::terminate(){
    lock();
    //if((PCB*)running != main_thread) return;

    delete timer;
    delete allThreads;
    delete allSemaphores;

    delete main_thread;
    unlock();
}

void System::block(){
    lock();
    timer->job = BLOCK;
    Timer::call();
    unlock();
}

void System::shutdown(){
    lock();
    timer->job = SHUTDOWN;
    timer->call();
    unlock();
}

void System::dispatch(){
    lock();
    timer->job = DISPATCH;
    Timer::call();
    unlock();
}



Thread* System::getThreadById(ID id){
	lock();
    if(System::allThreads->find(id) != 0){ unlock(); return (Thread*)(System::allThreads->find(id)->myThread);}
    else {unlock(); return 0;}
}

ID System::getRunningId(){
    return System::running->getId();
}

void System::putInScheduler(PCB* thread){
	lock();
	if(thread != ((PCB*)(System::idle))) {
		Scheduler::put(thread);
	}
	unlock();
}

PCB* System::getFromScheduler(){
	lock();
	PCB* temp = Scheduler::get();
	if(temp) {unlock(); return temp;}
	else {
		if(running_threads) {unlock(); return System::idle;}
		else {unlock(); return System::main_thread;}
	}
}

void System::blockSignalGlobally(SignalId signal){
	lock();
	System::signalsBlockedGlobally[signal] = 1;
	unlock();
}

void System::unblockSignalGlobally(SignalId signal){
	lock();
	System::signalsBlockedGlobally[signal] = 0;
	unlock();
}
