/*
 * pcb.h
 *
 *  Created on: Aug 14, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "thread.h"
#include "listPCB.h"

const StackSize maxStack = 65536;
const StackSize minStack = 256;

enum Type{IDLE, CREATED, READY, BLOCKED, FINISHED, FORKBLK};


class PCB {
public:
	volatile static PCB* running;
	volatile static PCB* child;
	static PCB* idle;
	volatile ListPCB* waitingOnMe;
	volatile ListPCB* myChildren;

	volatile int activeChildren;
	volatile PCB* parent;

	unsigned* stack;
	unsigned sp;
	unsigned ss;
	unsigned bp;

	Type threadType;
	int myTimeSlice;
	Thread *thread;
	int signalReleased;
	ID returnValue;
	unsigned long stackSize;

	PCB(StackSize stackSize, Time timeSlice, Thread* myThread);
	~PCB();

	void start();
	void waitToComplete();
	ID getId();

	static void wrapper();
	static ID getRunningId();
	static Thread * getThreadById(ID id);
	static void interrupt fork();
	static void exit();
	static void waitForForkChildren();


private:

	volatile static ListPCB* AllPCBs;
	volatile static ID startingID;
	int myID;
};



#endif /* PCB_H_ */
