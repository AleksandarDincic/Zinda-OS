/*
 * KernelSem.h
 *
 *  Created on: Apr 24, 2020
 *      Author: OS1
 */

#ifndef KRNLSEM_H_
#define KRNLSEM_H_
#include "List.h"
#include "SemList.h"
#include "Semaphor.h"

class KernelSem{
private:
	static SemList* semList;

	int val;
	ThreadList *blocked;
	SemList::SemNode *semListNode;

	KernelSem(int initVal);
	~KernelSem();

	int wait (Time maxTimeToWait);
    int signal(int n);

	void dec();
	friend class Semaphore;
	friend class SemList;
	friend void interrupt timer(...);
	friend int main(int argc, char* argv[]);
};



#endif /* KRNLSEM_H_ */
