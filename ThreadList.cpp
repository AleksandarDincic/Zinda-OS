/*
 * ThreadList.cpp
 *
 *  Created on: Apr 9, 2020
 *      Author: OS1
 */

#include "PCB.h"

ThreadList::Node* ThreadList::add(PCB *t, Time waitTime){
	Node *newNode = new Node(t, 0, 0, waitTime);
	if(!head){
		head = tail = newNode;
	}
	else{
		newNode->prev=tail;
		tail=tail->next=newNode;
	}
	return newNode;
}

PCB* ThreadList::getByID(ID id){
	Node *curr = head;
	while(curr){
		if(curr->thread->id==id)
			return curr->thread;
		curr=curr->next;
	}
	return 0;
}

void ThreadList::remove(Node *node){
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

PCB* ThreadList::removeFirst(){
	if(!head) return 0;

	PCB *temp = head->thread;
	Node *tempNode = head->next;

	delete head;
	head = tempNode;
	if(!head) tail = head;
	return temp;
}

ThreadList::~ThreadList(){
	while(head){
		Node *temp = head;

		head = head->next;
		delete temp;
	}
}

void ThreadList::globalWipe(){
	while(PCB::masterList->head){
		Node *temp = PCB::masterList->head;

		PCB::masterList->head = PCB::masterList->head->next;
		temp->thread->masterListNode=0;

		delete temp;
	}

	//brises sve iz master liste
}
