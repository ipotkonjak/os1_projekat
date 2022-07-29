/*
 * listPCB.h
 *
 *  Created on: Aug 15, 2021
 *      Author: OS1
 */

#ifndef LISTPCB_H_
#define LISTPCB_H_

class PCB;
class Thread;

class ListPCB {
private:
	typedef struct node{
		PCB* pcb;
		struct node* next;
		node(PCB* info){
			pcb = info;
			next = 0;
		}
	}Node;
	Node* head;
	Node* tail;
	Node* current;
public:
	ListPCB();
	~ListPCB();
	int numberOfNodes;
	int put(PCB* p);
	PCB* remove(int id);
	Thread* getThread(int id);
	PCB* get();
	void showList();

	void onFirst();
	void move();
	PCB* getTmpPCB();
};




#endif /* LISTPCB_H_ */
