/*
 * Dummy.h
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#ifndef DUMMY_H_
#define DUMMY_H_
#include "PCB.h"
#include <dos.h>

class Dummy : public PCB{
private:
	static void dummyRun(){
		while(1);
	}
public:
	Dummy(): PCB(defaultStackSize, 1, 0){
		stack = new unsigned[this->stackSize];
		stack[this->stackSize - 1] = 0x200;

		stack[this->stackSize - 2] = FP_SEG(Dummy::dummyRun);
		stack[this->stackSize - 3] = FP_OFF(Dummy::dummyRun);

		sp = FP_OFF(stack+this->stackSize-12);
		ss = FP_SEG(stack+this->stackSize-12);

		currentStatus = READY;
	}
};



#endif /* DUMMY_H_ */
