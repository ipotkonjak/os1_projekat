/*
 * ivtentry.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#include "funkcije.h"
#include "ivtentry.h"
#include <dos.h>
#include "thread.h"
IVTEntry* IVTEntry::AllIVTObjects[NumOfEntries] = {0};

IVTEntry::IVTEntry(IVTNo num, INTR newIntr) {
	closeLock
	planted = newIntr;
	this->num = num;
	linkedEvent = 0;
	AllIVTObjects[num] = this;

	asm pushf;
	asm cli;
	old = getvect(num);
	setvect(num, planted);
	asm popf;
	openLock
}
IVTEntry::~IVTEntry() {

	removeEvent();

}

void IVTEntry::signal() {
	if(linkedEvent) linkedEvent->signal();
}
void IVTEntry::setEvent(KernelEv *event) {
	asm pushf;
	asm cli;

	linkedEvent = event;

	unlockI
}

void IVTEntry::removeEvent() {
	asm pushf;
	asm cli;

	setvect(num, old);
	linkedEvent = 0;
	AllIVTObjects[num] = 0;

	unlockI

}

