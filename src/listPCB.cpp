/*
 * listPCB.cpp
 *
 *  Created on: Aug 15, 2021
 *      Author: OS1
 */
#include "listPCB.h"
#include "SCHEDULE.H"
#include "funkcije.h"
#include "pcb.h"
#include <stdio.h>
ListPCB::ListPCB(){
	closeLock
	head = current = tail = 0;
	numberOfNodes = 0;
	openLock
}

ListPCB::~ListPCB(){

	Node* old;
	closeLock
	while(head){

		if(head->pcb->threadType == BLOCKED) {
			head->pcb->threadType = READY;
			Scheduler::put(head->pcb);
		}

		old = head;
		head = head->next;
		old->pcb = 0;
		old->next = 0;
		delete old;

	}
	numberOfNodes = 0;
	head = current = tail = 0;

	openLock
}

int ListPCB::put(PCB* p){

	closeLock
	Node* curr = new Node(p);
	if(curr == 0) {
		openLock
		return -1;
	}
	if(head == 0) head = curr;
	else tail->next = curr;
	tail = curr;
	numberOfNodes++;
	openLock
	return 0;

}


PCB* ListPCB::get(){
	if(head == 0) return 0;
	closeLock
	Node* curr = head;
	head = head->next;
	numberOfNodes--;
	if(head == 0) tail = 0;
	PCB* tmp = curr->pcb;
	curr->pcb = 0;
	curr->next = 0;
	delete curr;
	openLock
	return tmp;
}

PCB* ListPCB::remove(ID id){//ne menja type
	if(head == 0) return 0;
	closeLock
	Node* curr = head,* pred = 0;

	while(curr != 0){
		if(curr->pcb->getId() == id) break;
		pred = curr;
		curr = curr->next;
	}
	if(curr == 0) {
		openLock
		return 0;
	}
	if(pred != 0)pred->next = curr->next;
	else head = curr->next;
	if(tail == curr) tail = (curr->next != 0 ? curr->next : pred);
	curr->next = 0;
	numberOfNodes--;
	PCB* tmp = curr->pcb;
	curr->pcb = 0;
	delete curr;
	openLock
	return tmp;
}

Thread* ListPCB::getThread(ID id){
	if(head == 0) return 0;
	closeLock
	Node* curr = head;
	while(curr!=0){
		if(curr->pcb->getId() == id) break;
		curr = curr->next;
	}
	if(curr == 0) {
		openLock
		return 0;
	}
	openLock
	return curr->pcb->thread;
}
void ListPCB::showList() {
	Node* tmp = head;
	printf("Num of nodes %i\n", numberOfNodes);
	while(tmp) {
		printf("Node: %i\n", tmp->pcb->getId());
		tmp = tmp->next;
	}
}

void ListPCB::onFirst() {current = head;}
void ListPCB::move() {
	if(current) current = current->next;
}
PCB* ListPCB::getTmpPCB() {
	if(current) return current->pcb;
	return 0;
}

