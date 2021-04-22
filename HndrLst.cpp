/*
 * HndrLst.cpp
 *
 *  Created on: May 17, 2020
 *      Author: OS1
 */

#include "HndrLst.h"


void HandlerList::add(SignalHandler h){
	HandlerNode *newNode = new HandlerNode(h);
	if(!head){
		head = tail = newNode;
	}
	else{
		newNode->prev=tail;
		tail=tail->next=newNode;
	}
}

void HandlerList::wipe(){
	while(head){
		HandlerNode *temp = head;

		head = head->next;
		delete temp;
	}
	tail = head;
}


void HandlerList::swap(SignalHandler h1, SignalHandler h2){
	HandlerNode* node1 = 0, *node2 = 0;
	HandlerNode* current = head;
	while(current){
		if(!node1 && current->handler == h1){
			node1 = current;
		}
		if(!node2 && current->handler == h2){
			node2 = current;
		}
		if(node1 && node2)
			break;
		current = current->next;
	}
	if(node1 && node2){
		SignalHandler t = node1->handler;
		node1->handler = node2->handler;
		node2->handler = t;
	}
}

void HandlerList::doAll(){
	HandlerNode *current = head;

	while(current){
		current->handler();
		current = current->next;
	}
}

HandlerList::~HandlerList(){
	wipe();
}

HandlerList::HandlerList(const HandlerList& l) : head(0), tail(0){
	HandlerNode* current = l.head;
	while(current){
		add(current->handler);
		current = current->next;
	}
}
