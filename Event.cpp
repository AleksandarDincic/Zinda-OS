/*
 * Event.cpp
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */
#include "KernelEv.h"
#include "PCB.h"

Event::Event (IVTNo ivtNo){
	lock
	myImpl = new KernelEv(ivtNo);
	IVTEntry::getEntry(ivtNo)->setEvent(myImpl);
	unlock

	//napravi KernelEv, i ubaci se u zadati IVTEntry
}

void Event::wait(){
	lock
	myImpl->wait();
	unlock
}

void Event::signal(){
	lock
	myImpl->signal();
	unlock

	//ovo je beskorisno??
}

Event::~Event(){
	lock
	IVTNo tempNum = myImpl->num;
	if(IVTEntry::getEntry(tempNum)) //EXPERIMENTAL
		IVTEntry::getEntry(tempNum)->setEvent(0);
	delete myImpl;
	myImpl = 0;
	unlock
}
