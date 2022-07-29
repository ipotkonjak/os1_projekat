/*
 * listSEM.h
 *
 *  Created on: Aug 16, 2021
 *      Author: OS1
 */

#ifndef LISTSEM_H_
#define LISTSEM_H_

class KernelSem;

class ListSemaphore {

private:
	typedef struct node{
		struct node* next;
		KernelSem* semaphore;
		node(KernelSem* ks){

			semaphore = ks;

			next=0;
		}
	}Node;
	Node* head;
	Node* tail;

public:
	ListSemaphore();
	~ListSemaphore();

	int numberOfNodes;

	void put(KernelSem* ks);
	void signal();
	void remove(KernelSem* k);

};



#endif /* LISTSEM_H_ */
