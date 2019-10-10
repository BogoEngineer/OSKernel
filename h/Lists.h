#ifndef _lists_h_
#define _lists_h_
#include "System.h"
#include "Lists.h"
#include "PCB.h"
#include "Thread.h"
#include "Timer.h"
#include "KerSem.h"
#include "Semaphor.h"
#include <dos.h>

typedef int ID;
typedef unsigned SignalId;
typedef void (*SignalHandler)();

class Element{
public:  
    PCB* info;
    Element* next;

    Element(PCB* thread): info(thread), next(0){}
    ~Element(){}
};

class ThreadsList{
public:
    Element* head;
    
    PCB* get();
    void remove(PCB* thread);
    void add(PCB* thread);
    int isEmpty();
    PCB* find(ID id);

    void iterateBlockedLeft();

    ThreadsList(): head(0){}
    ~ThreadsList(){}
};

class SemElement{
public:
	KernelSem* info;
	SemElement* next;

	SemElement(KernelSem* ks): info(ks), next(0){}
	~SemElement(){}

};

class SemaphoreList{
public:
	SemElement* head;

	void remove(KernelSem* ks);
	void add(KernelSem* ks);
	void iterate();

	SemaphoreList(): head(0){}
	~SemaphoreList(){
		SemElement* temp = head;
		while(temp){
			head = temp;
			temp = temp->next;
			head->next = 0;
			delete head;
		}
	}
};

class SigElement{
public:
	SignalId info;
	SigElement *next;
	SigElement(SignalId i) : info(i), next(0) {}
	~SigElement(){}
};

class SignalList{
public:
	SigElement *head;

	void remove(SigElement *sig);
	void add(SigElement* sig);
	void execute(PCB* requester); //obradi sve signale koji nisu blokirani
	SignalList* clone();
	SigElement* take();

	SignalList(): head(0){}
	~SignalList(){
		SigElement* temp = head;
		while(temp){
			head = temp;
			temp = temp->next;
			head->next = 0;
			delete head;
		}
	}
};

class HandlerElement{
public:
	SignalHandler info;
	HandlerElement *next;
	HandlerElement(SignalHandler i) : info(i), next(0) {}
	~HandlerElement(){}
};

class HandlerList{
public:
	HandlerElement *head;

	void remove(HandlerElement *sig);
	void add(HandlerElement* he);
	void swap(SignalHandler hand1, SignalHandler hand2);
	void execute(); //izvrsi sve handlere
	HandlerList* clone();

	HandlerList(): head(0){}
	~HandlerList(){
		HandlerElement* temp = head;
		while(temp){
			head = temp;
			temp = temp->next;
			head->next = 0;
			delete head;
		}
	}
};
#endif
