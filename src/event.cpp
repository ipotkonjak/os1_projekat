/*
 * event.cpp
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#include "event.h"
#include "krnlevnt.h"
#include "funkcije.h"
#include "thread.h"

Event::Event(IVTNo ivtNo) {
	closeLock
	myImpl = new KernelEv(ivtNo);
	openLock
}
Event::~Event() {
	closeLock
	if(myImpl != 0) {
		delete myImpl;
		myImpl = 0;
	}
	openLock
}
void Event::wait() {
	if(myImpl) myImpl->wait();
}
void Event::signal() {
	if(myImpl) myImpl->signal();
}

