#define __AVR_ATmega328P__
#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#include "uart.h"
//#include <stdio.h>
#include <util/delay.h>
//DEAL WITH RX INTERUPTS

//static FILE mystdout = FDEV_SETUP_STREAM(usart_putchar_printf, NULL, _FDEV_SETUP_WRITE); //serial out to debug

volatile static unsigned char rx_buffer[RX_BUFFER_SIZE] = {0};
volatile static uint16_t rx_count = 0;	
volatile static uint16_t read_pos = 0;

int main(){
    init_usart(2400,1);
    //stdout=&mystdout; //serial out to debug
    //DDRB|=1<<DDB5; //to check with inbuilt led, led me to finding sei not called.
    // PORTB&=!(1<<PORTB5); //to check if interrupts work
    sei();
    while(1);
    return 0;
}
ISR(USART_RX_vect){
    //PORTB^=(1<<PORTB5);
    //printf("%u\n",(unsigned int)(rx_count));
    char data=rcv_data();
    if(rx_count<RX_BUFFER_SIZE)
            rx_buffer[rx_count++]=data;
        else
            rx_count=0;
    if(data=='$'){
        //transmit_byte(rx_buffer[read_pos++]);
        send_string(rx_buffer);
        //rx_count=0;
    }
}

//instead of using send_string, can use interrupt. The interupts will recursively cause eachother until whole string is TXed. 
// ISR(USART_TX_vect){
//     if(read_pos<rx_count-1){
//         transmit_byte(rx_buffer[read_pos++]);
//     }
// }

void init_usart(uint32_t ubr,uint8_t async){// 32 bits to account for higher than 16 bits 
    //following documentation pg148 atmega328p
    int ubrr=((F_CPU/(16*ubr)) -1);
    UBRR0H=(unsigned char)(ubrr>>8);
    UBRR0L=(unsigned char)ubrr;
    //default uart mode, change UCSR0C[7:6] locs given by UMSEL00 AND UMSEL01
    if(async==0){
        UCSR0C|=1<<UMSEL00;
    }
    UCSR0C|=(1<<UCSZ01)|(1<<UCSZ00); //8 BIT DATA FRAME
    //ENABLE TRANSMITTER RECIEVER and respective interupts
    UCSR0B= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);
    _delay_ms(100);
}

void transmit_byte(volatile unsigned char data){
    while (!(UCSR0A & (1<<UDRE0)));//need to wait for buffer to be free, might cause hazards otherwise
    UDR0=data;
}

void send_string(volatile unsigned char *s){
    int i=0;
    while(s[i]!='\0')
        transmit_byte(s[i++]);
}

char rcv_data(){
    /* Wait for data to be received */
    while (!(UCSR0A & (1<<RXC0)));
/* Get and return received data from buffer */
    return (char)UDR0;
}
// int usart_putchar_printf(volatile unsigned char var, FILE *stream) {
//     if (var == '\n') transmit_byte('\r');
//     transmit_byte(var);
//     return 0;
// }



