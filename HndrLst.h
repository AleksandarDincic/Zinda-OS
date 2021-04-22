/*
 * HndrLst.h
 *
 *  Created on: May 17, 2020
 *      Author: OS1
 */

#ifndef HNDRLST_H_
#define HNDRLST_H_

typedef void (*SignalHandler)();

class HandlerList{
private:
	struct HandlerNode{
		SignalHandler handler;
		HandlerNode* prev, *next;
		HandlerNode(SignalHandler h, HandlerNode* n = 0, HandlerNode *p = 0) : handler(h), prev(p), next(n){}
	};

	HandlerNode *head, *tail;

	friend int main(int agrc, char* argv[]);
	friend void interrupt timer(...);

public:
	HandlerList() : head(0), tail(0){}
	HandlerList(const HandlerList& l);

	void add(SignalHandler h);
	void wipe();
	void swap(SignalHandler h1, SignalHandler h2);
	void doAll();

	~HandlerList();

};

#endif /* HNDRLST_H_ */
