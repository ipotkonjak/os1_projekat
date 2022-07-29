/*
 * idle.h*/
#ifndef IDLE_H_
#define IDLE_H_
#include <stdio.h>
#include "thread.h"
#include "pcb.h"

class Idle:public Thread{
public:
	Idle():Thread(4096,1){
		myPCB->threadType = IDLE;
	}
	void run(){
		while(1) {}
	}
};



#endif /* IDLE_H_ */
