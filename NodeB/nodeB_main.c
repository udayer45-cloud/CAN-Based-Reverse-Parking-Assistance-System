#include <LPC21xx.H>
#include "header.h"

volatile df rx;
volatile int f1=0,f2=0;

int main(){
	int res;
	int active=0;
	df tx;

	IODIR0|=(1<<3);    //p0.3 trig output
	IODIR0&=~(1<<4);   //p0.4 echo input
	IOCLR0=(1<<3);     //trig idle low

	can2_init();
	uart0_init(9600);
	config_can2_rx();
	config_timer0();
	config_timer0_itr();

	uart0_tx_string("Node B Ready\r\n");

	while(1){

		if(f1){
			f1=0;
			if(rx.rtr==1){
				uart0_tx_string("RTR received\r\n");
				active=1;
				T0TCR=1;
			}
			else{
				uart0_tx_string("Stop received\r\n");
				active=0;
				T0TCR=0;
				T0TC=0;
			}
		}

		if(f2 && active){
			f2=0;
			res=hc_sr04();

			uart0_tx_string("dist=");
			uart0_integer(res);
			uart0_tx_string("cm\r\n");

			tx.id=0x501;
			tx.rtr=0;
			tx.ff=0;
			tx.dlc=4;
			tx.byteA=res;
			can2_tx(tx);
		}
	}
}
