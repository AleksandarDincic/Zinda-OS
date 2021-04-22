/*
 * IVTEntry.h
 *
 *  Created on: May 5, 2020
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

typedef unsigned char IVTNo;
typedef void interrupt (*intRoutine)(...);

class KernelEv;
extern void dispatch();

class IVTEntry{
private:
	static IVTEntry* IVTETable[256];
	IVTNo num;
	KernelEv* currentEvent;

	void setEvent(KernelEv *event);
	intRoutine oldInt;

	friend class Event;
public:
	static IVTEntry* getEntry(IVTNo num);

	IVTEntry(IVTNo num, intRoutine newInt);
	~IVTEntry();

	void callOldInt();
	void signal();
};


#define PREPAREENTRY(num, oldInt)\
	void interrupt newInt##num(...);\
	IVTEntry IVTEntry##num(num,newInt##num);\
	void interrupt newInt##num(...){\
		IVTEntry##num.signal();\
		if(oldInt) IVTEntry##num.callOldInt();\
		dispatch();\
	}

#endif /* IVTENTRY_H_ */
