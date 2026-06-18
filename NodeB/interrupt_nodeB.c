#include <LPC21xx.H>
#include "header.h"

extern volatile df rx;
extern volatile int f1,f2;

void timer0_isr(void) __irq{
	f2=1;
	T0IR=1;
	VICVectAddr=0;
}

void config_timer0(){
	VICIntSelect=0;
	VICVectCntl1=4|(1<<5);
	VICVectAddr1=(ui)timer0_isr;
	VICIntEnable|=(1<<4);
}

void config_timer0_itr(){
	int a[]={15,60,30,15};
	int pclk=a[VPBDIV]*1000;
	T0TCR=2;
	T0PR=pclk-1;
	T0TC=0;
	T0MR0=60;
	T0MCR=3;
	T0TCR=0;
}

void can2_isr(void) __irq{
	rx.id=C2RID;
	rx.rtr=(C2RFS>>30)&1;
	rx.ff=(C2RFS>>31)&1;
	rx.dlc=(C2RFS>>16)&0xf;
	if(rx.rtr==0)
		rx.byteA=C2RDA;
	C2CMR=(1<<2);
	f1=1;
	VICVectAddr=0;
}

void config_can2_rx(){
	C2IER=1;
	VICIntSelect=0;
	VICVectCntl0=27|(1<<5);
	VICVectAddr0=(ui)can2_isr;
	VICIntEnable=(1<<27);
}
