#include <LPC21xx.H>
#include "header.h"

void delay_ms(ui ms){
	int a[]={15,60,30,15};
	int pclk=a[VPBDIV];
	T0TCR=2;
	T0PR=pclk-1;
	T0TC=0;
	T0TCR=1;
	while(T0TC<(ms*1000));
	T0TCR=0;
}
