/*
 * PCB.h
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "List.h"
#include "KrnlSem.h"

#define lock asm{\
	pushf;\
	cli;\
}

#define unlock asm popf

//#define lock asm cli

//#define unlock asm sti

class KernelEv;
class SignalList;
class HandlerList;

class PCB{
private:
	static ThreadList* masterList;
	static PCB* running;
protected:
	enum status {NEWBORN, READY, BLOCKED, FINISHED};
	static ID nextId;

	unsigned *stack;
	unsigned sp, ss;

	ID id;

	ThreadList* waiting;

	StackSize stackSize;

	status currentStatus;

	Time timeLeft;
	Time timeSlice;

	Thread* myThread;
	ThreadList::Node *masterListNode;

	int semRetVal;
	int toBeDestroyed;


	int signalAllowance[16];
	ID parentThreadID;
	SignalList* signalList;
	HandlerList* handlerList[16];

	void processSignals();

	friend class Thread;
	PCB(StackSize stackSize, Time timeSlice, Thread *owner);
	static void run();
	static void finish();
	static void setAsMain(Thread* t);
	~PCB();

	friend void interrupt timer(...);
	friend void processSignalsWrapper();
	friend int main(int agrc, char* argv[]);
	friend ThreadList;
	friend KernelSem;
	friend KernelEv;
};

#endif /* PCB_H_ */
