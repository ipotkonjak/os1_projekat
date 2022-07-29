/*
 * userthrd.cpp
 *
 *  Created on: Aug 24, 2021
 *      Author: OS1
 */

#include "userthrd.h"

int userMain(int argc, char* argv[]);

void UserThread::run() {
	status = userMain(argc,argv);
}
Thread* UserThread::clone() const {
	return new UserThread(argc, argv);
}

