#ifndef HEADER_H
#define HEADER_H

typedef unsigned char uc;
typedef unsigned int ui;
typedef unsigned short int usi;

typedef struct{
	ui id;
	uc rtr;
	uc ff;
	uc dlc;
	ui byteA;
	ui byteB;
}df;

void can2_init(void);
void can2_tx(df t);
void config_can2_rx(void);
void config_timer0(void);
void config_timer0_itr(void);
ui hc_sr04(void);
void delay_us(int us);
void uart0_init(ui baud);
void uart0_tx(uc data);
uc uart0_rx(void);
void uart0_tx_string(char *p);
void uart0_integer(int num);

#endif
