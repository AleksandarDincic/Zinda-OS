/*
 * SgnlLst.cpp
 *
 *  Created on: May 17, 2020
 *      Author: OS1
 */

#include "SgnlLst.h"

extern int globalSignalAllowance[16];

SignalList::SignalNode* SignalList::add(SignalId s){
	SignalNode *newNode = new SignalNode(s);
	if(!head){
		head = tail = newNode;
	}
	else{
		newNode->prev=tail;
		tail=tail->next=newNode;
	}
	signalNum++;
	return newNode;
}

void SignalList::remove(SignalNode *node){
	if(!node) return;

	if(node->next){
		node->next->prev = node->prev;
	}
	else{
		tail = node->prev;
	}

	if(node->prev){
		node->prev->next = node->next;
	}
	else{
		head = node->next;
	}

	signalNum--;
	delete node;
}

SignalId SignalList::removeFirst(int allowance[16]){
	SignalNode* currentNode = head;

	while(currentNode){

		if(globalSignalAllowance[currentNode->signal] && allowance[currentNode->signal]){
			SignalId retVal = currentNode->signal;
			remove(currentNode);
			return retVal;
		}

		currentNode = currentNode->next;
	}

	return 16;
}

SignalList::~SignalList(){
	signalNum = 0;
	while(head){
		SignalNode *temp = head;

		head = head->next;
		delete temp;
	}
}
