/*
 * pcb.cpp
 *
 *  Created on: Aug 14, 2021
 *      Author: OS1
 */
#include "pcb.h"
#include "idle.h"
#include "SCHEDULE.H"
#include "funkcije.h"
#include <dos.h>
#include <string.h>
#include <stdio.h>

class ListPCB;

volatile ListPCB* PCB::AllPCBs = new ListPCB();
volatile int PCB::startingID = 0;
volatile PCB* PCB::running = new PCB(4096,2,0);
PCB* PCB::idle = (new Idle())->myPCB;
volatile PCB* PCB::child = 0;


PCB::PCB(StackSize stackSize,Time timeSlice,Thread* myThread) {
	closeLock
	//printf("pravim nit %i\n", PCB::startingID);
	if (stackSize < minStack) stackSize = minStack;
	if (stackSize > maxStack) stackSize = maxStack;

	unsigned long actualSize = stackSize/sizeof(unsigned);

	stack = new unsigned[actualSize];
	if(stack == 0) {
		openLock
		return;
	}
	this->stackSize = actualSize;
	myID = startingID++;
	thread = myThread;
	myTimeSlice = timeSlice;
	signalReleased = 0;

	waitingOnMe = 0;
	if(AllPCBs->put(this)) {
		openLock
		return;
	}
	waitingOnMe = new ListPCB();
	if(waitingOnMe == 0) {
		openLock
		return;
	}
	myChildren = new ListPCB();
	if(myChildren == 0) {
		openLock
		return;
	}
	activeChildren = 0;

#ifndef BCC_BLOCK_IGNORE
	stack[actualSize-1]=0x200;
	stack[actualSize-2]=FP_SEG(PCB::wrapper);
	stack[actualSize-3]=FP_OFF(PCB::wrapper);
	ss=FP_SEG(stack+actualSize-12);
	sp=FP_OFF(stack+actualSize-12);
	bp=sp;
	stack[actualSize - 12] = 0;
#endif


	if(myID|0){
			threadType = CREATED;
			parent = (PCB*) running;
		}else {
			threadType = READY;
			parent = 0;
		}
//LISTA PCB

	//nlockCout
	openLock
}

PCB::~PCB() {
	//lockCout
	closeLock
	if(stack != 0) delete stack;
	stack = 0;
	if(waitingOnMe != 0) delete waitingOnMe;
	waitingOnMe = 0;
	if(AllPCBs != 0) AllPCBs->remove(myID);

	if(myChildren != 0) {
		myChildren->onFirst();
		PCB* tmp = myChildren->getTmpPCB();
		while(tmp) {
			tmp->parent = 0;
			myChildren->remove(tmp->getId());
			myChildren->onFirst();
			tmp = myChildren->getTmpPCB();
		}
	}
	myChildren = 0;
	if(parent) {
		if(parent->myChildren)parent->myChildren->remove(myID);
	}

	openLock
	//unlockCout
}

void PCB::start() {
	closeLock
	if(threadType == CREATED) {
		threadType = READY;
		Scheduler::put(this);
	}
	openLock

}
void PCB::waitToComplete() {
		closeLock
		if (PCB::running != this && threadType != FINISHED && threadType != CREATED && threadType != IDLE) {
			PCB::running->threadType = BLOCKED;
			waitingOnMe->put((PCB*)PCB::running);
			openLock
			dispatch();
		}
		else openLock;

}
ID PCB::getId() {
	return myID;
}
ID PCB::getRunningId() {//static
	return running->myID;
}
Thread * PCB::getThreadById(ID id) {//static
	//closeLock
	if(AllPCBs == 0) return 0;
	return AllPCBs->getThread(id);
	//openLock
}

void PCB::wrapper(){
	running->thread->run();
	closeLock
	running->threadType=FINISHED;
	if(running->waitingOnMe != 0) delete running->waitingOnMe;
	running->waitingOnMe = 0;

	if(running->parent != 0) {
		if(running->parent->activeChildren > 0) {running->parent->activeChildren--;
		//printf("moj roditelj je %i i ima %i dece, a ja sam %i\n", running->parent->getId(), running->parent->activeChildren, running->getId());
		}
		if(running->parent->activeChildren <= 0 && running->parent->threadType == FORKBLK) {
				running->parent->threadType = READY;
				Scheduler::put((PCB*)running->parent);
		}
	}
	else printf("Nemam roditelje a ja sam %i\n", running->getId());
	openLock

	dispatch();
}
void interrupt PCB::fork() {
	closeLock
	memcpy(PCB::child->stack, running->stack, running->stackSize*sizeof(unsigned));

	unsigned mySP;
	unsigned childVal, parentVal;
	unsigned* childSP,* parentSP;
	asm mov mySP, bp;

	PCB::child->sp = mySP + FP_OFF(PCB::child->stack) - FP_OFF(running->stack);
	childVal = PCB::child->sp;
	parentVal = mySP;

	parentSP = (unsigned*)MK_FP(running->ss, parentVal);
	childSP = (unsigned*)MK_FP(child->ss, childVal);

	while(*parentSP != 0) {
		*childSP = *parentSP - parentVal + childVal;

		childVal = *childSP;
		parentVal = *parentSP;

		parentSP = (unsigned*)MK_FP(running->ss, parentVal);
		childSP = (unsigned*)MK_FP(child->ss, childVal);
	}
	*childSP = 0;
	openLock

}
void PCB::exit() {
	closeLock
	running->threadType = FINISHED;
	if(running->waitingOnMe != 0) delete running->waitingOnMe;
	running->waitingOnMe = 0;

	if(running->parent) {
			if(running->parent->activeChildren > 0) running->parent->activeChildren--;

			if(running->parent->activeChildren == 0 && running->parent->threadType == FORKBLK) {
					running->parent->threadType = READY;
					Scheduler::put((PCB*)running->parent);
			}
		}
	openLock

	dispatch();
}
void PCB::waitForForkChildren() {

	closeLock
	if(running->myChildren == 0) {
		openLock
		return;
	}
	running->myChildren->onFirst();
	PCB* tmp = running->myChildren->getTmpPCB();

	running->activeChildren = 0;
	while(tmp != 0) {
		if((PCB*)PCB::running != tmp && tmp->threadType != FINISHED && tmp->threadType != CREATED && tmp->threadType != IDLE) {
			//if(tmp->waitingOnMe->put((PCB*)running) == 0)
				running->activeChildren++;


		}
		running->myChildren->move();
		tmp = running->myChildren->getTmpPCB();
	}
	if(running->activeChildren > 0) {
		//printf("Imam %i dece nit %i \n", running->activeChildren, getRunningId());
		//printf("Cekam %i dece %i\n", running->activeChildren, getRunningId());

		running->threadType = FORKBLK;
		openLock
		dispatch();
	}
	else openLock

}
