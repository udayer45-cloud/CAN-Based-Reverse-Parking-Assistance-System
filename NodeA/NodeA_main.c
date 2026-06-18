#include <LPC21xx.H>
#include "header.h"

volatile df temp;
volatile int f1=0,f2=0;

int main(){
	int buzz_delay=50;
	int gear=0;
	df s;

	IODIR0|=(1<<21);   //buzzer pin output
	IOCLR0=(1<<21);    //buzzer off initially

	can_init();
	uart0_init(9600);
	config_can_rx();
	config_ext0();

	uart0_tx_string("Car Started\r\n");

	while(1){

		if(f2){
			f2=0;
			gear^=1;

			s.id=0x501;
			s.ff=0;

			if(gear==1){
				uart0_tx_string("Reverse Gear Engaged\r\n");
				s.rtr=1;
				s.dlc=0;
				s.a=0;
			}
			else{
				uart0_tx_string("Reverse Gear Disengaged\r\n");
				IOCLR0=(1<<21);
				s.rtr=0;
				s.dlc=4;
				s.a=0x1;
			}
			tx_nodeA(s);
		}

		if(f1){
			f1=0;
			uart0_tx_string("dist=");
			uart0_integer((int)temp.a);
			uart0_tx_string("cm\r\n");

			if(temp.a>300)
				buzz_delay=1000;
			else if(temp.a<=300 && temp.a>200)
				buzz_delay=500;
			else if(temp.a<=200 && temp.a>100)
				buzz_delay=250;
			else
				buzz_delay=100;

			IOSET0=(1<<21);
			delay_ms(buzz_delay);
			IOCLR0=(1<<21);
			delay_ms(buzz_delay);
		}
	}
}
