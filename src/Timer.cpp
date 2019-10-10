#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include <dos.h>

#include <iostream.h>



volatile unsigned tsp;
volatile unsigned tss;
volatile unsigned tbp;

volatile int time_slice_flag;

extern void tick();

int Timer::thread_time = 0; //na pocetku ne treba da dodje do promene konteksta
char Timer::change_flag = 0;
long long unsigned int Timer::time = 0;


void interrupt timer(){ //proveri ovo
	Timer* local_timer = System::timer;
	Job local_job = System::timer->job;
	function_pointer_void local_task = local_timer->task[local_job];

	asm {
						mov tsp, sp
						mov tss, ss
						mov tbp, bp
		}
	System::running->sp = tsp;
	System::running->ss = tss;
	System::running->bp = tbp;

	local_task();

	if(Timer::change_flag == 1){
		System::running = System::getFromScheduler();

		tsp = System::running->sp;
		tss = System::running->ss;
		tbp = System::running->bp;

		asm {
							mov sp, tsp
							mov ss, tss
							mov bp, tbp
			}

		Timer::thread_time = Timer::time + System::running->time_slice;
		if(System::running->time_slice){
			time_slice_flag = 1;
		}else{
			time_slice_flag = 0;
		}
		Timer::change_flag = 0;
	}


	((SignalList*)System::running->pendingSignals)->execute(System::running); //handleSignals

	System::timer->job = STOPWATCH;
}


unsigned oldTimerOFF, oldTimerSEG;
void Timer::override(){
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0x0022
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0x0020
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0x0022, seg timer
		mov word ptr es:0x0020, offset timer

		mov ax, oldTimerSEG
		mov word ptr es:0x0182, ax
		mov ax, oldTimerOFF
		mov word ptr es:0x0180, ax

		pop ax
		pop es
		sti 
	}

}
void Timer::restore(){
    asm {
		cli
		push es
		push ax
		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0x0022, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0x0020, ax

		pop ax
		pop es
		sti
	}
}



void Timer::call(){
	asm int 8h;
}

void Timer::stopwatch(){//treba da proveri da l je isteklo vreme za razna cekanja po semaforima
	time++;
	System::allSemaphores->iterate(); //netesstirano
	tick();
	asm int 60h;
	if(time >= thread_time && time_slice_flag) {
	    Timer::dispatch();
	}
}
void Timer::block(){
    System::running->state = BLOCKED;
    if(System::linker != 0) ((ThreadsList*)(System::linker))->add(System::running);
    System::linker = 0;
    change_flag = 1;
}
void Timer::shutdown(){
}
void Timer::dispatch(){
	change_flag = 1;
	if(System::running == System::idle) return;
	if(System::stopProcess) { //trebalo bi da radi
		System::stopProcess = 0;
		((SignalList*)System::running->pendingSignals)->execute(System::running);
		System::running->myThread = 0;
		delete System::running->myThread;
		return;
	}
	if(System::running->state != FINISHED) System::putInScheduler(System::running);
}

Timer::Timer(){
	Job job = STOPWATCH;
	task[0] = &Timer::stopwatch;
	task[1] = &Timer::block;
	task[2] = &Timer::shutdown;
	task[3] = &Timer::dispatch;

    override();
}

Timer::~Timer(){
    restore();
}


