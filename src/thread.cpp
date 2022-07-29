/*
 * thread.cpp

 *
 *  Created on: Aug 14, 2021
 *      Author: OS1
 */

#include <dos.h>
#include "thread.h"
#include "funkcije.h"
#include "pcb.h"
#include <iostream.h>
#include <stdio.h>
#include "ASSERT.H"


Thread::Thread(StackSize stackSize, Time timeSlice) {
	closeLock
	myPCB = new PCB(stackSize, timeSlice, this);
	openLock
}
Thread::~Thread() {
	waitToComplete();
	closeLock
	if (myPCB != 0)
		delete myPCB;
	myPCB = 0;
	openLock
}
void Thread::start() {
	//printf("start %i\n", myPCB->getId());
	if (myPCB != 0)
		myPCB->start();
}
void Thread::waitToComplete() {
	if (myPCB != 0)
		myPCB->waitToComplete();
}
ID Thread::getId() {
	if (myPCB != 0)
		return myPCB->getId();
	else
		return -1;
}
ID Thread::getRunningId() { //static
	return PCB::getRunningId();
}
Thread * Thread::getThreadById(ID id) { //static
	return PCB::getThreadById(id);
}
void dispatch() {
	asm {
		pushf
		cli
	}

	//assert(lockFlag == 0);
	zahtevana_promena_konteksta = 1;
	timer();
	unlockI
}

ID Thread::fork() {

	closeLock
	Thread* childT = getThreadById(getRunningId())->clone();

	if(childT == 0) {
		openLock
		return -1;
	}
	if(childT->myPCB == 0) {
		openLock
		return -1;
	}
	if(childT->myPCB->stack == 0 || childT->myPCB->myChildren == 0 || childT->myPCB->waitingOnMe == 0){
		openLock
		return -1;
	}

	PCB::child = childT->myPCB;

	childT->myPCB->returnValue = 0;
	PCB::running->returnValue = childT->getId();
	if(PCB::running->myChildren->put(childT->myPCB)) {
		//delete childT;
		PCB::child = 0;
		openLock
		return -1;
	}

	PCB::fork();

	if(PCB::running != childT->myPCB) {
		childT->start();
		openLock
	}




	return PCB::running->returnValue;
}

void Thread::exit() {
	PCB::exit();
}
void Thread::waitForForkChildren() {
	PCB::waitForForkChildren();
}
