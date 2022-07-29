/*
 * semaphor.cpp
 *
 *  Created on: Aug 16, 2021
 *      Author: OS1
 */
#include "semaphor.h"
#include "funkcije.h"
#include "krnlsem.h"
#include "thread.h"

Semaphore::Semaphore (int init) {
	closeLock
	myImpl = new KernelSem(init, this);
	openLock
}
Semaphore::~Semaphore() {
	closeLock
	if(myImpl != 0) {
		delete myImpl;
		myImpl = 0;
	}
	openLock
}

int Semaphore::wait (int maxTimeToWait) {
	return myImpl->wait(maxTimeToWait);
}
void Semaphore::signal() {
	myImpl->signal();
}
int Semaphore::val () const {
	return myImpl->val();
}



