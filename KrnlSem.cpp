/*
 * KrnlSem.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */
#include "KrnlSem.h"
#include "PCB.h"
#include "SCHEDULE.H"

KernelSem::KernelSem(int initVal) : val(initVal){
	blocked = new ThreadList();
	semListNode = semList->add(this);
}

KernelSem::~KernelSem(){

	semList->remove(semListNode);
	semListNode=0;

	PCB *temp;

	while(temp = blocked->removeFirst()){
		temp->currentStatus = PCB::READY;
		Scheduler::put(temp);
	}

}

int KernelSem::wait (Time maxTimeToWait){
	lock
	if(--val <0){
		PCB::running->currentStatus=PCB::BLOCKED;
		blocked->add(PCB::running, maxTimeToWait);
		dispatch();
		unlock
		return PCB::running->semRetVal;
	}
	else{
		unlock
		return 1;
	}
}

int KernelSem::signal (int n){
	lock
	if(n < 0){
		unlock
		return n;
	}
	else if (n == 0){
		if(++val <= 0){
			PCB* temp = blocked->removeFirst();
			temp->currentStatus = PCB::READY;
			Scheduler::put(temp);
		}
		unlock
		return 0;
	}
	else {
		val+=n;
		int retVal = 0;
		PCB* temp = 0;
		while(retVal < n){
			temp = blocked->removeFirst();
			if(!temp) break;
			temp->currentStatus = PCB::READY;
			Scheduler::put(temp);
			retVal++;
		}
		unlock
		return retVal;
	}
}

void KernelSem::dec(){
	ThreadList::Node *curr = blocked->head;
	while(curr){
		ThreadList::Node *temp = curr->next;
		if(curr->timeLeft>0 && --(curr->timeLeft)==0) {
			curr->thread->semRetVal = 0;
			curr->thread->currentStatus = PCB::READY;
			Scheduler::put(curr->thread);
			blocked->remove(curr);
			val++;
		}
		curr=temp;
	}
}
