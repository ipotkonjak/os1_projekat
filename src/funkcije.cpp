/*
 * funkcije.cpp

 *
 *  Created on: Aug 14, 2021
 *      Author: OS1
 */
#include "funkcije.h"
#include "pcb.h"
#include "SCHEDULE.H"
#include "krnlsem.h"

unsigned oldTimerOFF;
unsigned oldTimerSEG;

extern void tick();

volatile unsigned int lockFlag = 0;

unsigned tsp;
unsigned tss;
unsigned tbp;

volatile int brojac = 2;
volatile int zahtevana_promena_konteksta = 0;

void interrupt timer(){	// prekidna rutina
	if (!zahtevana_promena_konteksta && brojac > 0) {
		brojac--;
	}
	if(!zahtevana_promena_konteksta) {
		if(lockFlag == 0)KernelSem::signalAll();
		asm int 60h;
		tick();
	}
	if (brojac == 0 && PCB::running->myTimeSlice != 0 || zahtevana_promena_konteksta) {
		if (lockFlag == 0) {
			zahtevana_promena_konteksta = 0;
		asm {
			// cuva sp
			mov tsp, sp
			mov tss, ss
			mov tbp, bp
		}

		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;


		if(PCB::running->threadType == READY) Scheduler::put((PCB*)PCB::running);

		PCB::running = Scheduler::get();
		if(PCB::running == 0) PCB::running = PCB::idle;

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

		brojac = PCB::running->myTimeSlice;

		asm {
			mov sp, tsp   // restore sp
			mov ss, tss
			mov bp, tbp
		}
		}
		else {
			zahtevana_promena_konteksta = 1;
		}
	}

	// poziv stare prekidne rutine koja se
     // nalazila na 08h, a sad je na 60h
     // poziva se samo kada nije zahtevana promena
     // konteksta – tako se da se stara
     // rutina poziva samo kada je stvarno doslo do prekida


	//zahtevana_promena_konteksta = 0;
}



void inic(){
	asm{
		cli
		push es
		push ax

		mov ax,0   //  ; inicijalizuje rutinu za tajmer
		mov es,ax

		mov ax, word ptr es:0022h //; pamti staru rutinu
		mov word ptr oldTimerSEG, ax
		mov ax, word ptr es:0020h
		mov word ptr oldTimerOFF, ax

		mov word ptr es:0022h, seg timer	 //postavlja
		mov word ptr es:0020h, offset timer //novu rutinu

		mov ax, oldTimerSEG	 //	postavlja staru rutinu
		mov word ptr es:0182h, ax //; na int 60h
		mov ax, oldTimerOFF
		mov word ptr es:0180h, ax

		pop ax
		pop es
		sti
	}
}

// vraca staru prekidnu rutinu
void restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

