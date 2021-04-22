/*
 * KernelEv.cpp
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */
#include "KernelEv.h"
#include "SCHEDULE.h"


KernelEv::KernelEv(IVTNo num) : num(num){
	ownerPCB = PCB::running;
	waiting = 0;
}

KernelEv::~KernelEv(){
	signal();
	//da li da stavim ovde signal? nek ide zivot
}

void KernelEv::wait(){
	if(PCB::running==ownerPCB && !waiting){
		ownerPCB->currentStatus=PCB::BLOCKED;
		waiting = 1;
		dispatch();
	}
}

void KernelEv::signal(){
	if(waiting){
		ownerPCB->currentStatus=PCB::READY;
		Scheduler::put(ownerPCB);
		waiting = 0;
	}
}
