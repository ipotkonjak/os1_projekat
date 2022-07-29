/*
 * krnlsem.cpp
 *
 *  Created on: Aug 16, 2021
 *      Author: OS1
 */
#include "listPCB.h"
#include "SCHEDULE.H"
#include "funkcije.h"
#include "krnlsem.h"
#include "pcb.h"
#include "listSEM.h"
#include "listTIME.h"
#include <dos.h>
#include <stdio.h>

ListSemaphore* KernelSem::AllSemaphores = new ListSemaphore();

void KernelSem::signalAll() {

	closeLock
	KernelSem::AllSemaphores->signal();
	openLock

}

KernelSem::KernelSem(int startingValue, Semaphore* sem) {
	closeLock
	value = startingValue;
	mySemaphore = sem;

	timerWait = new ListTimer();
	infinityWait = new ListPCB();

	KernelSem::AllSemaphores->put(this);
	openLock

}

KernelSem::~KernelSem() {
	closeLock
	KernelSem::AllSemaphores->remove(this);

	if(timerWait != 0) {
		delete timerWait;
		timerWait = 0;
	}
	if(infinityWait != 0) {
		delete infinityWait;
		infinityWait = 0;
	}
	openLock
}

void KernelSem::signal() {
	closeLock

	value++;
	if(value<=0){
		PCB* setFree = infinityWait->get();


		if(setFree == 0) {
			asm pushf;
			asm cli;
			setFree = timerWait->get();
			asm popf;
		}
		if(setFree == 0) return;

		setFree->signalReleased = 1;
		setFree->threadType = READY;
		Scheduler::put(setFree);
		}
	openLock

}
int KernelSem::wait(int timeToWait) {

	int ret;
	closeLock
	value--;
	if(value<0){
		PCB::running->threadType = BLOCKED;
		if(timeToWait == 0) infinityWait->put((PCB*)PCB::running);
		else timerWait->put((PCB*)PCB::running, this, timeToWait);

	openLock
	dispatch();

	closeLock
	ret = PCB::running->signalReleased;
	PCB::running->signalReleased = 0;
	openLock
	return ret;

	}
	else {
		openLock
		return 1;
	}
}
int KernelSem::val() const { return value; }
void KernelSem::editVal() {
	closeLock
	value++;
	openLock
}


