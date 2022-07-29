/*
 * listTIME.cpp
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#include "listTIME.h"
#include "pcb.h"
#include "krnlsem.h"
#include "SCHEDULE.H"
#include "funkcije.h"
#include <stdio.h>

ListTimer::ListTimer() {
	closeLock
	head = tail = 0;
	numberOfNodes = 0;
	openLock
}

ListTimer::~ListTimer() {
	Node* old;
	closeLock
	while(head){

		if(head->pcb->threadType == BLOCKED) {
			head->pcb->threadType = READY;
			head->pcb->signalReleased = 0;
			Scheduler::put((PCB*)head->pcb);
		}
		old = head;
		head = head->next;
		old->pcb = 0;
		old->next = 0;
		old->mySemaphore = 0;
		delete old;

		}
		numberOfNodes = 0;
		head = tail = 0;
	openLock
}


void ListTimer::put(PCB* p, KernelSem* ks, int t) {

	closeLock
	Node* newNode = new Node(p, ks, t);
	if(newNode == 0) return;
	if(head) {
		Node* curr = head,* pred = 0;
		while(newNode->blockTime >= curr->blockTime) {
			newNode->blockTime -= curr->blockTime;

			pred = curr;
			curr = curr->next;
			if(curr == 0) break;
		}

		if(pred) pred->next = newNode;
		else head = newNode;
		newNode->next = curr;

		if(curr && newNode->blockTime) {
			curr->blockTime -= newNode->blockTime;
		}
		if(curr == 0) tail = newNode;


	}
	else head = tail = newNode;
	numberOfNodes++;
	openLock
}

void ListTimer::signal() {
	//closeLock
	//printf("signal\n");
	//closeLock
	closeLock
	if(head) {
		Node* old;
		if(head->blockTime>0)head->blockTime--;
		while(head->blockTime == 0) {
			head->mySemaphore->editVal();
			head->pcb->threadType = READY;
			head->pcb->signalReleased = 0;
			Scheduler::put(head->pcb);

			old = head;
			head = head->next;
			old->pcb = 0;
			old->next = 0;
			old->mySemaphore = 0;
			delete old;
			numberOfNodes--;

			if(head == 0) {
				tail = 0;
				break;
			}
		}
	}
	//openLock
	openLock
	//openLock
}

PCB* ListTimer::get() {

	//printf("get\n");
	asm pushf;
	asm cli;
	if(head == 0) {
		unlockI
		return 0;
	}
	Node* setFree = head;
	head = head->next;
	if(head) {
		head->blockTime += setFree->blockTime;
		setFree->next = 0;
	}
	else tail = 0;
	numberOfNodes--;

	PCB* ret = setFree->pcb;
	setFree->pcb = 0;
	setFree->mySemaphore = 0;
	delete setFree;
	unlockI
	return ret;

}
