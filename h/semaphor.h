/*
 * semaphor.h
 *
 *  Created on: Aug 16, 2021
 *      Author: OS1
 */

#ifndef SEMAPHOR_H_
#define SEMAPHOR_H_

class KernelSem;
class Semaphore {
public:

	Semaphore (int init=1);
	virtual ~Semaphore ();

	virtual int wait (int maxTimeToWait);
	virtual void signal();
	int val () const; // Returns the current value of the semaphore

private:
 KernelSem* myImpl;

};

#endif /* SEMAPHOR_H_ */
