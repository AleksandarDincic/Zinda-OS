/*
 * main.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "Dummy.h"
#include "Semaphor.h"

unsigned oldTimerOFF, oldTimerSEG;

extern Dummy* dummyPCB;

class BlankThread : public Thread {
};

void interrupt timer(...);
void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es, ax

		mov ax, word ptr es:0022h
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer
		mov word ptr es:0020h, offset timer


		//ovo je ulaz 60h, i stavljas tu staru prekidnu rutinu
		//da bi je pozivao iz nove (moras ovo)
		mov ax, oldTimerSEG
		mov word ptr es:0182h, ax
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

void restore(){
	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

extern int userMain (int argc, char* argv[]);

int main(int argc, char* argv[]){
	inic();

	//ponisti promene ovde ako puca nesto (ne bi trebalo)
	Thread* mainThread = new BlankThread();
	PCB::setAsMain(mainThread);

	int retVal = userMain(argc, argv);

	restore();
	ThreadList::globalWipe();
	SemList::globalWipe();
	delete PCB::masterList;
	delete KernelSem::semList;
	delete dummyPCB;
	return retVal;
}
