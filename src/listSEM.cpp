/*
 * listSEM.cpp
 *
 *  Created on: Aug 16, 2021
 *      Author: OS1
 */

#include "listSEM.h"
#include "krnlsem.h"
#include "listTIME.h"
#include "funkcije.h"
#include "thread.h"

ListSemaphore::ListSemaphore() {
	closeLock
	head = tail = 0;
	numberOfNodes = 0;
	openLock
}

ListSemaphore::~ListSemaphore() {
	Node* old;
	closeLock
	while(head) {
		old = head;
		head = head->next;
		old->next = 0;
		old->semaphore = 0;

		delete old;
	}
	head = tail = 0;
	numberOfNodes = 0;
	openLock
}

void ListSemaphore::put(KernelSem* k) {
	closeLock
	Node* newNode = new Node(k);
	if(newNode == 0) {
		openLock
		return;
	}
	if(head) tail = tail->next = newNode;
	else head = tail = newNode;
	numberOfNodes++;
	openLock
}

void ListSemaphore::signal() {
	closeLock
	Node* curr = head;
	while(curr) {
		curr->semaphore->timerWait->signal();
		curr = curr->next;
	}
	openLock
}

void ListSemaphore::remove(KernelSem* k) {
	if(head == 0) return;
	closeLock
	Node* curr = head,* pred = 0;
	while(curr->semaphore != k) {
		pred = curr;
		curr = curr->next;
		if(curr == 0) break;
	}
	if(curr == 0){
		openLock
		return;
	}
	if(pred) pred->next =curr->next;
	else head = curr->next;
	numberOfNodes--;
	if(curr == tail) tail = pred;
	if(curr) {
		curr->semaphore = 0;
		curr->next = 0;
		delete curr;
	}
	openLock
}
