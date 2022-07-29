/*
 * ivtentry.h
 *
 *  Created on: Aug 19, 2021
 *      Author: OS1
 */

#ifndef IVTENTRY_H_
#define IVTENTRY_H_

#include "krnlevnt.h"


#define NumOfEntries 256
typedef void interrupt (*INTR) (...);

#define PREPAREENTRY(numEntry, flag)\
void interrupt inter##numEntry(...); \
extern IVTEntry newEntry##numEntry; \
void interrupt inter##numEntry(...) {\
newEntry##numEntry.signal();\
if (flag == 1)\
newEntry##numEntry.old();\
}\
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);

class IVTEntry {
public:

	static IVTEntry* AllIVTObjects[NumOfEntries];
	INTR old;

	IVTEntry(IVTNo num, INTR newIntr);
	~IVTEntry();

	void signal();
	void setEvent(KernelEv *event);
	void removeEvent();


private:
	KernelEv* linkedEvent;
	INTR planted;
	IVTNo num;
};
#endif
