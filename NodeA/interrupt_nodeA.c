#include <LPC21xx.H>
#include "header.h"

extern volatile df temp;
extern volatile int f1,f2;

void can2_isr() __irq{
	temp.id=C2RID;
	temp.rtr=(C2RFS>>30)&1;
	temp.ff=(C2RFS>>31)&1;
	temp.dlc=(C2RFS>>16)&0xf;
	temp.a=C2RDA;
	if(temp.dlc>4)
		temp.b=C2RDB;
	C2CMR=(1<<2);
	f1=1;
	VICVectAddr=0;
}

void ext0_isr() __irq{
	delay_ms(20);              //debounce
	if((IOPIN0&(1<<16))==0)    //confirm still pressed
		f2=1;
	EXTINT=1;
	VICVectAddr=0;
}

void config_can_rx(){
	C2IER=1;
	VICIntSelect=0;
	VICVectCntl0=27|(1<<5);
	VICVectAddr0=(ui)can2_isr;
	VICIntEnable=(1<<27);
}

void config_ext0(){
	PINSEL1|=1;
	EXTMODE|=1;
	EXTPOLAR=0;
	VICVectCntl1=14|(1<<5);
	VICVectAddr1=(ui)ext0_isr;
	VICIntEnable|=(1<<14);
}
