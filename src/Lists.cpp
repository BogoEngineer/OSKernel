#include "Lists.h"
#include "System.h"

PCB* ThreadsList::get(){
	if(head == 0) return 0;
	Element* ret = head;
	if(ret->info == 0) return 0;
	head = head->next;
	ret->next = 0;
	return ret->info;
}

void ThreadsList::add(PCB* thread){
    Element* add = new Element(thread);
    if(head == 0) {
    	head = add;
    }
    else{
        add->next = head;
        head = add;
    }
}
void ThreadsList::remove(PCB* thread){
    Element* curr = head;
    Element* bef = 0;
    if(curr == 0) return;
    while((PCB*)curr->info != thread){
        bef = curr;
        curr = curr->next;
        if(curr == 0) return;
    }
    if(curr != 0){
        if(curr == head){
        	head = head->next;
        	curr->next = 0;
        }
        else{
            bef->next = curr->next;
            curr->next = 0;
        } 
    }
}

int ThreadsList::isEmpty(){
	if(head == 0) return 1;
	else return 0;
}

PCB* ThreadsList::find(ID id){
	Element *curr = head;
	while(curr!= 0){
		if(curr->info->getId() == id) break;
		curr = curr->next;
	}
	if(curr != 0) return curr->info;
	else return 0;
}

void ThreadsList::iterateBlockedLeft(){
	Element *temp = head;
	PCB *ready;
	while(temp != 0){
		if((--(temp->info->blockedLeft)) == 0){
			ready = temp->info;
			ready->state = READY;
			ready->waitingFlag = 1;
			System::putInScheduler(ready);
			this->remove(temp->info);
		}
		temp = temp->next;
	}
}

void SemaphoreList::add(KernelSem* ks){
	SemElement* add = new SemElement(ks);
	    if(head == 0) {
	    	head = add;
	    }
	    else{
	        add->next = head;
	        head = add;
	    }
}

void SemaphoreList::remove(KernelSem* ks){
	 SemElement* curr = head;
	 SemElement* bef = 0;
	 if(curr == 0) return;
	 while((KernelSem*)curr->info != ks){
	     bef = curr;
	     curr = curr->next;
	     if(curr == 0) return;
	 }
     if(curr != 0){
    	 if(bef == 0) {
    		 head = head->next;
    		 curr->next = 0;
    	 }
	     else{
	            bef->next = curr->next;
	            curr->next = 0;
	        }
	 }
}

void SemaphoreList::iterate(){
	SemElement* temp = head;
	while(temp != 0){
		((ThreadsList*)temp->info->blockedThreads)->iterateBlockedLeft();
		temp = temp->next;
	}
}

void SignalList::remove(SigElement *sig){ //ne koristim, verovatno je faulty
	 SigElement* curr = head;
	 SigElement* bef = 0;
	 if(curr == 0) return;
	 while(curr != sig){
	     bef = curr;
	     curr = curr->next;
	     if(curr == 0) return;
	 }
    if(curr != 0){
    	if(bef == 0) {
    		head = head->next;
    		curr->next = 0;
    	}
	    else{
	            bef->next = curr->next;
	            curr->next = 0;
	    }
    }
}

void SignalList::add(SigElement* sig){ //dodaje na kraj liste da bi se izvrsavali redom kojim su ubaceni
	sig->next = 0;

	if(head == 0) {
		head = sig;
	}
	else{
		SigElement *temp = head;
	    while(temp->next) temp = temp->next;
	    temp->next = sig;
	}
}

void SignalList::execute(PCB* requester){
	SigElement* curr;
	SignalList *executent = new SignalList();

	do{
		curr = ((SignalList*)requester->pendingSignals)->take();
		if(curr == 0) break;
		if((requester->signalsBlockedLocally[curr->info] == 0) && (System::signalsBlockedGlobally[curr->info] == 0)){
			((HandlerList*)requester->allHandlers[curr->info])->execute();
			delete curr;
		}else{
			executent->add(curr);
		}
	}while(curr != 0);
	requester->pendingSignals = executent;
	delete this;
}

SignalList* SignalList::clone(){
	SignalList* ret = new SignalList();

	SigElement *temp = head;
	while(temp){
		ret->add(new SigElement(temp->info));
		temp = temp->next;
	}

	return ret;
}

SigElement* SignalList::take(){
	SigElement *ret;
	if(head == 0) return 0;
	ret = head;
	head = head->next;
	ret->next = 0;
	return ret;
}

void HandlerList::remove(HandlerElement *sig){
	 HandlerElement* curr = head;
	 HandlerElement* bef = 0;
	 if(curr == 0) return;
	 while(curr != sig){
	     bef = curr;
	     curr = curr->next;
	     if(curr == 0) return;
	 }
   if(curr != 0){
	   if(bef == 0){
   		head = head->next;
   		curr->next = 0;
	   }
	   else{
	            bef->next = curr->next;
	            curr->next = 0;
	   }
   }
}

void HandlerList::add(HandlerElement* he){ //dodaje na kraj liste da bi se izvrsavali redom kojim su ubaceni
	he->next = 0;

	if(head == 0) {
		head = he;
	}
	else{
		HandlerElement *temp = head;
	    while(temp->next) temp = temp->next;
	    temp->next = he;
	}
}

void HandlerList::swap(SignalHandler hand1, SignalHandler hand2){
	if(hand1 == hand2) return;
	if(hand1 == 0 || hand2 == 0) return;
	HandlerElement *pos1 = 0, *pos2 = 0, *temp = head;
	while(temp){
		if(temp->info == hand1) pos1 = temp;
		if(temp->info == hand2) pos2 = temp;
		temp = temp->next;
	}

	if(pos1 == 0 || pos2 == 0) return;
	SignalHandler pebble = pos1->info;
	pos1->info = pos2->info;
	pos2->info = pebble;
}

void HandlerList::execute(){
	HandlerElement* temp = head;
	while(temp){
		(temp->info)();
		temp = temp->next;
	}
}

HandlerList* HandlerList::clone(){
	HandlerList* ret = new HandlerList();

	HandlerElement *temp = head;
	while(temp){
		ret->add(new HandlerElement(temp->info));
		temp = temp->next;
	}

	return ret;
}
