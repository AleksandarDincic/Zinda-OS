/*
 * KernelEv.h
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#ifndef KERNELEV_H_
#define KERNELEV_H_

#include "PCB.h"
#include "Event.h"

class KernelEv{
private:
	PCB* ownerPCB;
	IVTNo num;
	int waiting;

	void wait();
	void signal();
	KernelEv(IVTNo num);
	~KernelEv();

	friend class Event;
	friend class IVTEntry;
};



#endif /* KERNELEV_H_ */
