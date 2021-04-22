/*
 * IVTEntry.cpp
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */
#include "KernelEv.h"
#include "PCB.h"
#include <dos.h>

void IVTEntry::setEvent(KernelEv* event){
	currentEvent = event;
	//setuj event u entryju na prosledjeni
}


IVTEntry* IVTEntry::getEntry(IVTNo num){
	return IVTETable[num];
	//vrati IVTEntry u zadatom ulazu
}


IVTEntry::IVTEntry(IVTNo num, intRoutine newInt) : num(num){
	lock
	IVTETable[num] = this;
	currentEvent = 0;
	oldInt = getvect(num);
	setvect(num, newInt);
	unlock

	//Namesti ulaz u IVTEntry tabeli, setuj currentEvent na 0, sacuvaj staru prekidnu rutinu i namesti novu
}

IVTEntry::~IVTEntry(){
	lock
	IVTETable[num] = 0;
	setvect(num, oldInt);
	unlock

	//vrati staru prekidnu rutinu i pocisti ulaz u IVTEntry tabeli
}

void IVTEntry::signal(){
	if(currentEvent)
		currentEvent->signal();
	//pozovi Signal od Event
}

void IVTEntry::callOldInt(){
	oldInt();
}
