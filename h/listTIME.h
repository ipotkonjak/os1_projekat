/*
 * listTIME.h
 *
 *  Created on: Aug 17, 2021
 *      Author: OS1
 */

#ifndef LISTTIME_H_
#define LISTTIME_H_

class PCB;
class KernelSem;

class ListTimer {

private:
	typedef struct node{
		PCB* pcb;
		KernelSem* mySemaphore;
		int blockTime;
		struct node* next;

		node(PCB* p, KernelSem* ks, int t){
			pcb = p;
			mySemaphore = ks;
			blockTime = t;
			next=0;
		}
	}Node;
	Node* head;
	Node* tail;
public:
	ListTimer();
	~ListTimer();

	int numberOfNodes;

	void put(PCB* p, KernelSem* ks, int t);
	void signal();
	PCB* get();
};



#endif /* LISTTIME_H_ */
