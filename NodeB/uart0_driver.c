#include <LPC21xx.H>
#include "header.h"

/*
 * VPBDIV register encoding on LPC2129:
 * VPBDIV=0 -> PCLK = CCLK/4 = 15MHz
 * VPBDIV=1 -> PCLK = CCLK   = 60MHz  <-- we use this
 * VPBDIV=2 -> PCLK = CCLK/2 = 30MHz
 * VPBDIV=3 -> PCLK = CCLK/4 = 15MHz
 * so array index matches: {15, 60, 30, 15}
 */
void uart0_init(ui baud){
	int pclk,res;
	int a[]={15,60,30,15};
	pclk=a[VPBDIV]*1000000;
	res=pclk/(16*baud);
	PINSEL0|=0x5;
	U0LCR=0x83;
	U0DLL=res&0xff;
	U0DLM=(res>>8)&0xff;
	U0LCR=0x3;
}

#define THRE ((U0LSR>>5)&1)

void uart0_tx(uc data){
	while(THRE==0);
	U0THR=data;
}

#define RDR (U0LSR&1)

uc uart0_rx(void){
	while(RDR==0);
	return U0RBR;
}

void uart0_tx_string(char *p){
	while(*p){
		uart0_tx(*p);
		p++;
	}
}

void uart0_integer(int num){
	int i=0;
	char a[15];
	if(num==0){
		uart0_tx('0');
		return;
	}
	else if(num<0){
		uart0_tx('-');
		num=-num;
	}
	while(num){
		a[i]=num%10+48;
		num=num/10;
		i++;
	}
	for(i=i-1;i>=0;i--)
		uart0_tx(a[i]);
}
