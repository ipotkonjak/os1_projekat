/*
 * userthrd.h
 *
 *  Created on: Aug 24, 2021
 *      Author: OS1
 */

#ifndef USERTHRD_H_
#define USERTHRD_H_


#include "thread.h"

class UserThread : public Thread {
	int argc;
	char** argv;
	int status;

public:
	UserThread(int argc, char** argv) : Thread() {
		this->argc = argc;
		this->argv = argv;
		status = 0;
	}

	int getStatus() { return status; }
	~UserThread() { waitToComplete(); }

	Thread* clone() const;
protected:
	void run();
};


#endif /* USERTHRD_H_ */
