/*
 * Semaphore.cpp
 *
 *  Created on: Apr 25, 2020
 *      Author: OS1
 */
#include "PCB.h"
#include "KrnlSem.h"
#include "Semaphor.h"

Semaphore::Semaphore (int init) {
	lock
	myImpl = new KernelSem(init);
	unlock

	//Kreiraj objekat KernelSem sa zadatom vrednoscu
	//Ako budes imao listu semafora dodaj ga u listu
}

int Semaphore::wait(Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
	//LOCK UNLOCK UNUTRA!
	//smanji val za 1, ako je <=0 blokiraj ovu nit na dato vreme
	//NEMOJ DA SORTIRAS LISTU PO VREMENU, ZATO STO THREADOVI TREBA DA SE VADE REDOM KOJIM SU DODATI OSIM AKO IM NE ISTEKNE VREME!!
	//moraces da modifikujes listu da ima timeLeft
}

int Semaphore::signal(int n){
	return myImpl->signal(n);
	//LOCK UNLOCK UNUTRA!
	//izvadi koliko mozes threadova iz blokade i vrati u scheduler
}

int Semaphore::val() const {
	return myImpl->val;
	//vrati val
}

Semaphore::~Semaphore() {
	lock
	myImpl->signal(-myImpl->val);
	delete myImpl;
	unlock
	//obrisi KernelSem, ali da li osloboditi blokirane niti? (da)
}

