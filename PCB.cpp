/*
 * PCB.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include <dos.h>
#include "PCB.H"
#include "Dummy.h"
#include "SCHEDULE.h"
#include "SgnlLst.h"
#include "HndrLst.h"

Dummy* dummyPCB = new Dummy();

volatile unsigned tempSP = 0, tempSS = 0;
volatile int dispatched = 0;

volatile int contextSwitchAllowed = 1;

volatile int globalSignalAllowance[16] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

PCB::PCB(StackSize stackSize, Time timeSlice, Thread* owner){

	id = nextId++;
	this->stackSize = stackSize / sizeof(stackSize);
	this->timeSlice = timeSlice;
	timeLeft = timeSlice;
	masterListNode = 0;
	toBeDestroyed = 0;
	semRetVal = 0;
	waiting = 0;
	signalList = 0;

	if(owner){

		masterListNode = masterList->add(this);
		waiting = new ThreadList();

		stack = new unsigned[this->stackSize];
		stack[this->stackSize - 1] = 0x200;

		stack[this->stackSize - 2] = FP_SEG(PCB::run);
		stack[this->stackSize - 3] = FP_OFF(PCB::run);

		sp = FP_OFF(stack+this->stackSize-12);
		ss = FP_SEG(stack+this->stackSize-12);

		signalList = new SignalList();
	} //owner == 0 -> slucaj kada se radi o dummyPCB, koji ne dodajemo u master listu i podesavamo specijalan stek

	if(running){
		parentThreadID = running->id;
		for(int i = 0; i < 16; i++){
			signalAllowance[i] = running->signalAllowance[i];
			handlerList[i] = new HandlerList(*(running->handlerList[i]));
		}
	} else{
		parentThreadID = -1;
		for(int i = 0; i < 16; i++){
			signalAllowance[i] = 1;
			handlerList[i] = new HandlerList();
		}
	}

	currentStatus = NEWBORN;
	myThread = owner;
}


void PCB::run(){
	if(running->myThread){
		running->myThread->run();

		PCB::finish();
		dispatch();
	}

	//pozovi run iz threada vlasnika ovog PCB tj onog sto je trenutno running
	//kad zavrsi to stavi status finished i obrisi thread iz svega sto treba
}

void PCB::finish(){
	lock

	PCB *temp;

	while(temp = running->waiting->removeFirst()){
		temp->currentStatus = READY;
		Scheduler::put(temp);
	}

	Thread* parent = Thread::getThreadById(running->parentThreadID);
	//OVDE LOCK DODAJ AKO NE RADI

	int naturalFinish = 0;

	//u PCB::finish() moze da se dodje iz timera i kad nit zavrsi run, ako je promena konteksta dozvoljena onda si upao preko run
	if(contextSwitchAllowed){
		naturalFinish = 1;
		contextSwitchAllowed = 0;
	}
	//moras da disablujes context switch ovde za svaki slucaj, zato sto signal() ima unlock na kraju
	//cini mi se da ne moze nista lose da se desi, ali da ne brines...

	if(parent)
		parent->signal(1);

	running->myThread->signal(2);

	processSignalsWrapper();

	if(naturalFinish)
		contextSwitchAllowed = 1;

	//baci signale koji treba da se bace

	running->currentStatus=FINISHED;
	unlock
}

void PCB::setAsMain(Thread *t){
	delete t->myPCB->stack;
	t->myPCB->stack = 0;
	t->myPCB->currentStatus = READY;

	running = t->myPCB;

	//stek ti ne treba - obrisi ga
	//postavi ovu nit kao running
	//ovo ne moras da lockujes, zato sto ce da se pozove samo jednom (u mainu), i dokle god je running 0 nece doci do promene konteksta
}

SemList::SemNode* curr;
extern void tick();
volatile PCB* oldRunning;


void interrupt timer(...){
	if(!dispatched){ //mozda ovde da vratis uslov da semList postoji
		curr = KernelSem::semList->head;
		while(curr){
			curr->sem->dec();
			curr=curr->next;
		}
	}

	if(PCB::running && contextSwitchAllowed && (dispatched || (PCB::running->timeSlice && !--PCB::running->timeLeft))){
		PCB::running->timeLeft = PCB::running->timeSlice;
		asm{
			mov tempSP, sp
			mov tempSS, ss
		}
		PCB::running->sp = tempSP;
		PCB::running->ss = tempSS;

		oldRunning = PCB::running;

		if(PCB::running != dummyPCB && PCB::running->currentStatus == PCB::READY){
			Scheduler::put(PCB::running);
		}

		PCB::running = Scheduler::get();

		if(!PCB::running){
			PCB::running = dummyPCB;
		}

		tempSS = PCB::running->ss;
		tempSP = PCB::running->sp;

		asm{
			mov ss, tempSS
			mov sp, tempSP
		}

		if(oldRunning->toBeDestroyed)
			oldRunning->~PCB();

		if(PCB::running->signalList && PCB::running->signalList->signalNum != 0){

			contextSwitchAllowed = 0;

			if (!dispatched) {
				tick();
				asm int 60h;
			}

			dispatched = 0;

			processSignalsWrapper();

			contextSwitchAllowed = 1;

			dispatched = 1;

		}

	}
	if (!dispatched) {
		tick();
		asm int 60h;
	}

	dispatched = 0;
}

PCB::~PCB(){

	delete signalList;
	signalList = 0;

	for(int i = 0; i < 16; i++){
		delete handlerList[i];
		handlerList[i] = 0;
	}

	delete stack;
	stack = 0;

	delete waiting;
	waiting = 0;

	//obrises dinamicku memoriju samo
	//ne mora lock i unlock posto se samo zove iz Thread destructora koji vec ima lock i unlock
}

//SIGNALI

void processSignalsWrapper(){
	asm sti //HARD UNLOCK
	PCB::running->processSignals();
	asm cli //HARD LOCK

	//wrapper obican
}

void PCB::processSignals(){
	SignalId currentSignal;

	while((currentSignal=signalList->removeFirst(signalAllowance)) < 16){
		handlerList[currentSignal]->doAll();
		if(currentSignal == 0){
			PCB::finish();
			toBeDestroyed = 1;
			contextSwitchAllowed = 1;
			dispatch();
		}
	}

	//obradis sve signale sto mogu da se obrade
}
