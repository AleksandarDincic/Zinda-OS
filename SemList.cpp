/*
 * SemList.cpp
 *
 *  Created on: Apr 26, 2020
 *      Author: OS1
 */
#include "SemList.h"
#include "KrnlSem.h"

SemList::SemNode* SemList::add(KernelSem *t){
	SemNode *newNode = new SemNode(t);
	if(!head){
		head = tail = newNode;
	}
	else{
		newNode->prev=tail;
		tail=tail->next=newNode;
	}
	return newNode;
}

void SemList::remove(SemNode *node){
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

	delete node;
}

KernelSem* SemList::removeFirst(){
	if(!head) return 0;

	KernelSem *temp = head->sem;
	SemNode *tempNode = head->next;

	delete head;
	head = tempNode;
	if(!head) tail = head;
	return temp;
}

SemList::~SemList(){
	while(head){
		SemNode *temp = head;

		head = head->next;
		delete temp;
	}
}

void SemList::globalWipe(){
	if(KernelSem::semList){
		while(KernelSem::semList->head){
			SemNode *temp = KernelSem::semList->head;

			KernelSem::semList->head = KernelSem::semList->head->next;
			temp->sem->semListNode=0;

			delete temp;
		}
	}
	//brises sve iz master liste
}
