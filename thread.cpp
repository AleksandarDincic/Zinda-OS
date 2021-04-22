/*
  * thread.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "PCB.h"
#include "List.h"
#include "SCHEDULE.H"
#include "SgnlLst.h"
#include "HndrLst.h"

extern volatile int dispatched;
extern int globalSignalAllowance[16];

void Thread::start(){
	lock
	if(myPCB->currentStatus == PCB::NEWBORN){
		myPCB->currentStatus = PCB::READY;
		Scheduler::put(myPCB);
	}
	unlock
	//daj PCB scheduleru, to je to valjda
}

void Thread::waitToComplete(){
	lock
	if(myPCB->currentStatus!=PCB::FINISHED){
		PCB::running->currentStatus=PCB::BLOCKED;
		myPCB->waiting->add(PCB::running);

		dispatch();
	}
	unlock

	//Da se vrtis u petlji dok status ne bude finished?
	//	-gubljenje vremena
	//Da se cuvaju niti koje cekaju na ovu i onda da se vrate
	//u scheduler kad zavrsi?
	//  -bolje
}

Thread::~Thread(){
	lock
	if(myPCB->masterListNode){
		PCB::masterList->remove(myPCB->masterListNode);
		myPCB->masterListNode = 0;
	}
	myPCB->myThread = 0;

	delete myPCB;
	myPCB = 0;
	unlock
	//Wait to complete ne treba ovde, znaci samo obrisi dinamicke podatke?
}

ID Thread::getId(){
	return myPCB->id;

	//samo vrati id iz PCB
}

ID Thread::getRunningId(){
	return PCB::running->id;

	//samo vrati id od running
}

Thread* Thread::getThreadById(ID id){
	return PCB::masterList->getByID(id)->myThread;

	//da imas neku globalnu listu u kojoj ces da cuvas
	//PCB sve i da vadis i brises kako zatreba?
}

Thread::Thread (StackSize stackSize, Time timeSlice){
	lock
	myPCB = new PCB(stackSize, timeSlice, this);
	unlock
	//kreiraj PCB
}

void dispatch(){
	lock
	dispatched = 1;
	timer();
	unlock

	//eksplicitno pozovi timer i guess
}


//SIGNALI


void Thread::signal(SignalId signal){

	lock
	myPCB->signalList->add(signal);
	unlock

	//dodaj signal u listu signala za ovaj thread
}

void Thread::registerHandler(SignalId signal, SignalHandler handler){

	lock
	myPCB->handlerList[signal]->add(handler);
	unlock

	//dodaj handler na kraj liste handlera za dati signal
}


void Thread::unregisterAllHandlers(SignalId id) {

	lock
	myPCB->handlerList[id]->wipe();
	unlock

	//brisi sve handlere za dati signal
}

void Thread::swap(SignalId id, SignalHandler hand1, SignalHandler hand2){

	lock
	myPCB->handlerList[id]->swap(hand1, hand2);
	unlock

	//zameni mesta ovim signalima; napravi metodu za ovo u listi pa neka ti ovo bude wrapper
}

void Thread::blockSignal(SignalId signal) {

	myPCB->signalAllowance[signal] = 0;

	//blokiraj lokalno signal
}

void Thread::blockSignalGlobally(SignalId signal) {

	globalSignalAllowance[signal] = 0;

	//blokiraj globalno signal
}

void Thread::unblockSignal(SignalId signal) {

	myPCB->signalAllowance[signal] = 1;

	//odblokiraj lokalno signal
}

void Thread::unblockSignalGlobally(SignalId signal) {

	globalSignalAllowance[signal] = 1;

	//odblokiraj globalno signal
}
