#include <LPC21xx.H>
#include "header.h"

void delay_us(int us){
	int a[]={15,60,30,15};
	int pclk=a[VPBDIV];
	T1TCR=2;
	T1PR=pclk-1;
	T1TC=0;
	T1TCR=1;
	while(T1TC<(ui)us);
	T1TCR=2;
}
