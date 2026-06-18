#include <LPC21xx.H>
#include "header.h"

void can_init(){
	VPBDIV=1;
	PINSEL1|=0x00014000;
	C2MOD=1;
	C2BTR=0x001c001d;
	AFMR=2;
	C2MOD=0;
}

#define TCS ((C2GSR>>3)&1)

void tx_nodeA(df t){
	C2TID1=t.id;
	C2TFI1=((ui)t.rtr<<30)|((ui)t.dlc<<16);
	if(t.rtr==0)
		C2TDA1=t.a;
	C2CMR=0x21;
	while(TCS==0);
}
