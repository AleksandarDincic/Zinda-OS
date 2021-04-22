/*
 * 1A.cpp
 *
 *  Created on: May 21, 2020
 *      Author: OS1
 */

#include "KrnlSem.h"
#include "PCB.h"
#include "KernelEv.h"

IVTEntry* IVTEntry::IVTETable[256];

ID PCB::nextId = 0;
PCB* PCB::running = 0;

ThreadList* PCB::masterList = new ThreadList();
SemList* KernelSem::semList = new SemList();
