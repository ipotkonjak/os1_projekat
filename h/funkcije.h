/*
 * funkcije.h
 *
 *  Created on: Aug 14, 2021
 *      Author: OS1
 */

#ifndef FUNKCIJE_H_
#define FUNKCIJE_H_
extern volatile int zahtevana_promena_konteksta;
extern volatile unsigned int lockFlag;


#define lockI asm {\
				pushf\
				cli\
				}
#define unlockI asm popf;

#define closeLock ++lockFlag;
#define openLock --lockFlag;\
        if (lockFlag == 0 && zahtevana_promena_konteksta) {\
                dispatch();\
}

void inic();
void restore();
void interrupt timer();






#endif /* FUNKCIJE_H_ */
