/*
 * krnlevnt.h
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#ifndef KRNLEVNT_H_
#define KRNLEVNT_H_

#include "event.h"

class PCB;
class Semaphore;

class KernelEv {
public:
	KernelEv(IVTNo ivt);
	~KernelEv();

	void wait();
	void signal();

private:
	PCB* myPCB;

	Semaphore* mySEM;
	IVTNo number;
};

#endif /* KRNLEVNT_H_ */
