/*
 * krnlevnt.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#include "pcb.h"
#include "krnlevnt.h"
#include "funkcije.h"
#include "SCHEDULE.H"
#include "ivtentry.h"
#include "semaphor.h"
#include <stdio.h>
KernelEv::KernelEv(IVTNo ivt) {
	closeLock
	number = ivt;
	myPCB = (PCB*)PCB::running;

	IVTEntry::AllIVTObjects[number]->setEvent(this);
	mySEM = new Semaphore(0);
	openLock
}
KernelEv::~KernelEv() {
	closeLock
	if(IVTEntry::AllIVTObjects[number])  IVTEntry::AllIVTObjects[number]->removeEvent();
	if(mySEM) delete mySEM;
	openLock
}
void KernelEv::wait() {

	if(myPCB == (PCB*) PCB::running)mySEM->wait(0);

}

void KernelEv::signal() {

	if(mySEM->val() < 0)mySEM->signal();


}
