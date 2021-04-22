/*
 * SgnlLst.h
 *
 *  Created on: May 17, 2020
 *      Author: OS1
 */

#ifndef SGNLLST_H_
#define SGNLLST_H_

typedef unsigned SignalId;

class SignalList{
private:
	struct SignalNode{
		SignalId signal;
		SignalNode* prev, *next;
		SignalNode(SignalId s, SignalNode* n = 0, SignalNode *p = 0) : signal(s), prev(p), next(n){}
	};

	SignalNode *head, *tail;
	volatile int signalNum;

	friend int main(int agrc, char* argv[]);
	friend void interrupt timer(...);

public:
	SignalList() : signalNum(0), head(0), tail(0){ }

	SignalNode* add(SignalId s);
	void remove(SignalNode *node);
	SignalId removeFirst(int allowance[16]);

	~SignalList();
};




#endif /* SGNLLST_H_ */
