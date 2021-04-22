/*
 * SemList.h
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */

#ifndef SEMLIST_H_
#define SEMLIST_H_
class KernelSem;

class SemList{
private:
	struct SemNode{
		KernelSem* sem;
		SemNode* prev, *next;
		SemNode(KernelSem *t, SemNode* n = 0, SemNode *p = 0) : sem(t), prev(p), next(n){}
	};

	SemNode *head, *tail;

	static void globalWipe();

	friend int main(int agrc, char* argv[]);
	friend void interrupt timer(...);

public:
	SemList() : head(0), tail(0){}

	SemNode* add(KernelSem *t);
	void remove(SemNode *node);
	KernelSem* removeFirst();

	~SemList();
};



#endif /* SEMLIST_H_ */
