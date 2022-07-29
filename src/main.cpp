/*
 * main.cpp
 *
 *  Created on: Aug 15, 2021
 *      Author: OS1
 */
#include "funkcije.h"
#include "userthrd.h"
int userMain(int argc, char** argv);
int main(int argc, char** argv) {
	inic();
	Thread* nit = new UserThread( argc,  argv);
	nit->start();
	nit->waitToComplete();
	//userMain(argc, argv);
	restore();
}



