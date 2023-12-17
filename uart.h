#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#define RX_BUFFER_SIZE 1023
void init_usart(uint32_t ubr,uint8_t async);
void transmit_byte(unsigned char data);
void send_string(volatile unsigned char *s);
char rcv_data();
int usart_putchar_printf(volatile unsigned char var, FILE *stream);
int main();

