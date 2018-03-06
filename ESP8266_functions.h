/**
 * C library for the ESP8266 WiFi module with a PIC microcontroller
 * Copyright (C) 2015 Camil Staps <info@camilstaps.nl>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *****************************************************************************
 *ORIGINALLY CREATED AS
 * File:    esp8266.h
 * Author:  Camil Staps <info@camilstaps.nl>
 * Website: http://github.com/camilstaps/ESP8266_PIC
 * Version: 0.1
 * 
 * MODFIED AND IMPROVED BY
 * File:    esp8266_functions.h
 * Author:  Aswinth Raj B  <mailtoaswinth@gmail.com>
 * Website: circuitdigest.com
 * Version: 0.1
 *
 * See:   circuitdigest.com for more explanation
 *
 * This is the header file for the ESP8266 PIC16F877A library where ESP runs on 115200 baudrate. 
 *
 *  * 
 * MODFIED AND IMPROVED BY
 * File:    esp8266_functions.h
 * Author:  Phillipe Sedycias de Queiroz  <phillipesedycias@hotmail.com>
 * Website: www.philseque.com
 * Version: 0
 *
 * See:   circuitdigest.com for more explanation
 *
 * This is the header file for the ESP8266 PIC16F877A library where ESP runs on 115200 baudrate. 
 *
 */



#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "lcd.h"
#include <xc.h>


#define ESP8266_STATION 0x01
#define ESP8266_SOFTAP 0x02

#define ESP8266_TCP 1
#define ESP8266_UDP 0

#define ESP8266_OK 1
#define ESP8266_READY 2
#define ESP8266_FAIL 3
#define ESP8266_NOCHANGE 4
#define ESP8266_LINKED 5
#define ESP8266_UNLINK 6
#define LED1ON 1
#define LED1OFF 2
#define LED2ON 3
#define LED2OFF 4

int len;


///***______________________ESP module Function Declarations__________________**///
void _esp8266_putch(unsigned char);
unsigned char _esp8266_getch(void);
/** Function prototypes **/
bit esp8266_isStarted(void);        // Check if the module is started (AT)
bit esp8266_restart(void);          // Restart module (AT+RST)
void esp8266_echoCmds(bool);        // Enabled/disable command echoing (ATE)
// WIFI Mode (station/softAP/station+softAP) (AT+CWMODE)
void esp8266_mode(unsigned char);
// Connect to AP (AT+CWJAP)
void esp8266_connect(unsigned char*, unsigned char*);
// Disconnect from AP (AT+CWQAP)
void esp8266_disconnect(void);
// Local IP (AT+CIFSR)
void esp8266_ip(char*);
// Create connection (AT+CIPSTART)
bit esp8266_start(unsigned char protocol, char* ip, unsigned char port);
// Send data (AT+CIPSEND)
bit esp8266_send(unsigned char*);
// Receive data (+IPD)
void esp8266_receive(unsigned char*, uint16_t, bool);
/** Functions for internal use only **/
// Print a string to the output
void _esp8266_print(unsigned const char *);
// Wait for a certain string on the input
inline uint16_t _esp8266_waitFor(unsigned char *);
// Wait for any response on the input
inline unsigned char _esp8266_waitResponse(void);
inline unsigned char _esp8266_waitResponse2(void); 

void Lcd_Write_Char(char);
void Lcd_Wtrite_String(unsigned char *);
void Lcd_Clear();
void Lcd_Init();
void Lcd_Set_Cursor(unsigned char,unsigned char);
void Lcd_Cmd(unsigned char);
void Lcd_Port(unsigned char);

void _esp8266_login_mail(unsigned char*, unsigned char*);
void _esp8266_mail_sendID(unsigned char*);
void _esp8266_mail_recID(unsigned char*);
void _esp8266_mail_subject(unsigned char*);
void _esp8266_mail_body(unsigned char*);
void envia_pagina();
void recebe_pagina();
void inicia_esp8266();
//********__________________End of Function Declaration_________________********///






//***Initialize UART for ESP8266**//
void Initialize_ESP8266(void)
{
    //****Setting I/O pins for UART****//
    TRISBbits.TRISB2 = 0; // TX Pin set as output
    TRISBbits.TRISB1 = 1; // RX Pin set as input
    //________I/O pins set __________//
    
    /**Initialize SPBRG register for required 
    baud rate and set BRGH for fast baud_rate**/
    SPBRG = 10;
    BRGH  = 1;  // for high baud_rate
    //_________End of baud_rate setting_________//
    
    //****Enable Asynchronous serial port*******//
    SYNC  = 0;    // Asynchronous
    SPEN  = 1;    // Enable serial port pins
    //_____Asynchronous serial port enabled_______//
    //**Lets prepare for transmission & reception**//
    TXEN  = 1;    // enable transmission
    CREN  = 1;    // enable reception
    //__UART module up and ready for transmission and reception__//
    
    //**Select 8-bit mode**//  
    TX9   = 0;    // 8-bit reception selected
    RX9   = 0;    // 8-bit reception mode selected
    //__8-bit mode selected__//     
}
//________UART module Initialized__________//



 
//**Function to send one byte of date to UART **//
void _esp8266_putch(char bt)  //Encaminha um bute de dados para UART
{
    while(!TXIF);  // hold the program till TX buffer is free
                    //espera o transmissor ficar livre
    
    TXREG = bt; //Load the transmitter buffer with the received value
                //carrega o transmissor com o valor recebido bt
}
//_____________End of function________________//


 
//**Function to get one byte of date from UART**//
char _esp8266_getch()   
{

    if(OERR) // check for Error 
    {
        CREN = 0; //If error -> Reset 
        CREN = 1; //If error -> Reset 
    }
    
    while(!RCIF); // hold the program till RX buffer is free
                  //repera RX ficar livre
        
    return RCREG; //receive the value and send it to main function
                  //pega o valor recebido e emcaminha para função main
}
//_____________End of function________________//
