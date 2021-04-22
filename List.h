/*
 * List.h
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#ifndef LIST_H_
#define LIST_H_

#include "thread.h"

class ThreadList{
private:
	struct Node{
		PCB* thread;
		Time timeLeft;
		Node* prev, *next;
		Node(PCB *t, Node* n = 0, Node *p = 0, Time timeLeft = 0) : thread(t), prev(p),next(n),timeLeft(timeLeft){}
	};

	Node *head, *tail;

	static void globalWipe();

	friend int main(int agrc, char* argv[]);
	friend class KernelSem;
public:
	ThreadList() : head(0), tail(0){ }

	Node* add(PCB *t, Time waitTime = 0);
	PCB* getByID(ID id);
	void remove(Node *node);
	PCB* removeFirst();

	~ThreadList();
};



#endif /* LIST_H_ */
