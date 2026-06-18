#ifndef HEADER_H
#define HEADER_H

typedef unsigned int ui;
typedef unsigned char uc;
typedef unsigned short int usi;

typedef struct{
	usi id;
	uc rtr;
	uc ff;
	uc dlc;
	ui a;
	ui b;
}df;

void can_init(void);
void tx_nodeA(df t);
void config_can_rx(void);
void config_ext0(void);
void uart0_init(ui baud);
void uart0_tx(uc data);
uc uart0_rx(void);
void uart0_tx_string(char *p);
void uart0_integer(int num);
void uart0_float(float f);
void delay_ms(ui ms);

#endif
