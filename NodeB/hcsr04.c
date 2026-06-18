#include <LPC21xx.H>
#include "header.h"

//p0.3 = trig (output), p0.4 = echo (input)
#define ECHO ((IOPIN0>>4)&1)

ui hc_sr04(){
	ui res,des;
	ui timeout;
	int a[]={15,60,30,15};
	int pclk=a[VPBDIV];

	T1TCR=2;
	T1PR=pclk-1;   //1 tick = 1us
	T1TC=0;

	//send 10us trigger pulse
	IOCLR0=(1<<3);
	delay_us(2);
	IOSET0=(1<<3);
	delay_us(10);
	IOCLR0=(1<<3);

	//wait for echo high with timeout
	timeout=0;
	while(ECHO==0){
		timeout++;
		if(timeout>60000) return 999;   //no echo, return 999
	}

	T1TCR=1;   //start timer

	//wait for echo low with timeout
	timeout=0;
	while(ECHO==1){
		timeout++;
		if(timeout>60000){
			T1TCR=0;
			return 999;
		}
	}

	T1TCR=0;   //stop timer
	res=T1TC;
	des=(0.0343*res)/2;
	return des;
}
