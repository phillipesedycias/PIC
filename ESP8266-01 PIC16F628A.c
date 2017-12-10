#define _XTAL_FREQ 20000000

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "ESP8266_functions.h" //lch.h included in this library

// CONFIG
#pragma config FOSC = HS  // Oscillator Selection bits (INTOSC oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital input, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config LVP = OFF        // Low-Voltage Programming Enable bit (RB4/PGM pin has digital I/O function, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EE Memory Code Protection bit (Data memory code protection off)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

char i;

void main()
{    
    GIE = 1;     //ativando interrupções globais, para usar o USART
    PEIE = 1;    //ativando as interrupções periféricas, para usar o USART
    
    CMCON = 0x07;
 
    TRISB = 0x00;
    
    TRISA = 0b00001000;     //Bbotão na RA3
    
    Lcd_Init();            //Inicia LCD
    
    Lcd_Clear();
    
    PORTA=0;
    
    PORTB=0;
    
    Initialize_ESP8266();   //Inicializa ESP (configurações)
    
    /*Check if the ESP_PIC communication is successful*/
    do
    {
    //Lcd_Set_Cursor(1,1);
    //Lcd_Write_String("ESP OFF");
    //PORTAbits.RA0=~PORTAbits.RA0;
    __delay_ms(1000);
    
    }while (!esp8266_isStarted()); //wait till the ESP send back "OK"
   
    Lcd_Set_Cursor(1,1);
    Lcd_Write_String("ESP ON");
    
    inicia_esp8266();
        
    while(1)
    {
        for (i=0;i<=10;i++) {
        PORTAbits.RA0=1;
        __delay_ms(50);
        PORTAbits.RA0=0;
        __delay_ms(50);
        }
        
       envia_pagina(); 		

       recebe_pagina();    //recebe comando da pagina
   
    }
}
        
