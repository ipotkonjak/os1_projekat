/*
 * krnlsem.h
 *
 *  Created on: Aug 16, 2021
 *      Author: OS1
 */

#ifndef KRNLSEM_H_
#define KRNLSEM_H_

class Semaphore;
class ListPCB;
class ListTimer;
class ListSemaphore;

class KernelSem {
public:

	static ListSemaphore* AllSemaphores;

	Semaphore* mySemaphore;
	ListTimer* timerWait;
	ListPCB* infinityWait;

	static void signalAll();

	KernelSem(int startingValue, Semaphore* sem);
	~KernelSem();

	void signal();
	int wait(int timeToWait);
	int val() const;
	void editVal();

private:
	int value;
};




#endif /* KRNLSEM_H_ */
