/* Microchip Technology Inc. and its subsidiaries.  You may use this software 
 * and any derivatives exclusively with Microchip products. 
 * 
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS".  NO WARRANTIES, WHETHER 
 * EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED 
 * WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A 
 * PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION 
 * WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION. 
 *
 * IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
 * INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND 
 * WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS 
 * BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE 
 * FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS 
 * IN ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF 
 * ANY, THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE 
 * TERMS. 
 */

/* 
 * File:  lcd.h 
 * Author: Camil Staps | Electrosom | Circuitdigest
 * Comments: Library Adapted by LCD 16x4 and included new functions (clear line)
 * By: Phillipe Sedycias de Queiroz
 * www.philseque.com
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


//****************Definição dos pinos do PIC que irão no LCD****************
//************************PIC port settings on the LCD ****************
#define RS RB3  //ok
#define EN RB0  //ok
#define D4 RB4  //ok
#define D5 RB5  //ok
#define D6 RB6  //ok
#define D7 RB7  //ok
//****************Fim das definições dos pinos do PIC que irão no LCD****************

//****************Inicio das rotinas para LCD****************
void Lcd_Port(unsigned char a)
{
	if(a & 1)
		D4 = 1;
	else
		D4 = 0;

	if(a & 2)
		D5 = 1;
	else
		D5 = 0;

	if(a & 4)
		D6 = 1;
	else
		D6 = 0;

	if(a & 8)
		D7 = 1;
	else
		D7 = 0;
}
void Lcd_Cmd(unsigned char a)
{
	RS = 0;             // => RS = 0
	Lcd_Port(a);
	EN  = 1;             // => E = 1
        __delay_ms(4);
        EN  = 0;             // => E = 0
}

void Lcd_Clear()
{
	Lcd_Cmd(0);
	Lcd_Cmd(1);
}


//função para direcionamento do cursor (linha,coluna))
void Lcd_Set_Cursor(unsigned char a,unsigned char b)
{
	char temp,z,y;
	if(a == 1)
	{
	  temp = 0x80 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
	else if(a == 2)
	{
		temp = 0xC0 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
    else if(a == 3)
	{
		temp = 0x94 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
    else if(a == 4)
	{
		temp = 0xD4 + b - 1;
		z = temp>>4;
		y = temp & 0x0F;
		Lcd_Cmd(z);
		Lcd_Cmd(y);
	}
    
    
}

void Lcd_Init()
{
  Lcd_Port(0x00);
   __delay_ms(20);
  Lcd_Cmd(0x03);
	__delay_ms(5);
  Lcd_Cmd(0x03);
	__delay_ms(11);
  Lcd_Cmd(0x03);
  /////////////////////////////////////////////////////
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x02);
  Lcd_Cmd(0x08);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x0C);
  Lcd_Cmd(0x00);
  Lcd_Cmd(0x06);
}

void Lcd_Write_Char(unsigned char a)
{
   char temp,y;
   temp = a&0x0F;
   y = a&0xF0;
   RS = 1;             // => RS = 1
   Lcd_Port(y>>4);             //Data transfer
   EN = 1;
   __delay_us(40);
   EN = 0;
   Lcd_Port(temp);
   EN = 1;
   __delay_us(40);
   EN = 0;
}

void Lcd_Write_String(unsigned char *a)
{
	int i;
	for(i=0;a[i]!='\0';i++)
	   Lcd_Write_Char(a[i]);
}

/* Funções desativadas para reduzir memória - disaber becouse this pic no have memory*/

/*
void Lcd_Shift_Right()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x0C);
}

void Lcd_Shift_Left()
{
	Lcd_Cmd(0x01);
	Lcd_Cmd(0x08);
}
//****************Fim das rotinas para LCD****************




//Início da criação de caractere (função disponível no site electrosome.com)
const unsigned short MyChar5x8[] = {
  0x00, 0x00, 0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, // Code for char num #0
 
  0x0E, 0x1B, 0x11, 0x11, 0x11, 0x11, 0x1F, 0x00, // Code for char num #1
  0x0E, 0x1B, 0x11, 0x11, 0x11, 0x1F, 0x1F, 0x00, // Code for char num #2
  
  
  //0x0E, 0x1B, 0x11, 0x11, 0x1F, 0x1F, 0x1F, 0x00, // Code for char num #3
  0x00, 0x04, 0x0A, 0x08, 0x0E, 0x0E, 0x0E, 0x00,
 
  
  0x00, 0x00, 0x04, 0x0A, 0x0E, 0x0E, 0x0E, 0x00, // Code for char num #4
  
  
  //0x0E, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x00, // Code for char num #5
  0x1C, 0x1C, 0x10, 0x00, 0x1C, 0x18, 0x1C, 0x00, // Code for char num #5
  
  
  //0x00, 0x04, 0x02, 0x1F, 0x02, 0x04, 0x00, 0x00, // Code for char num #6
  0x10, 0x10, 0x18, 0x02, 0x12, 0x13, 0x10, 0x00, // Code for char num #6
  
  
  
  //0x00, 0x00, 0x0E, 0x11, 0x11, 0x0A, 0x1B, 0x00  // Code for char num #7
  0x1C, 0x1C, 0x10, 0x05, 0x17, 0x15, 0x10, 0x00  // Code for char num #7 (personalizado conforme imagem abaixo)
  
};

void InitCustomChars() //(função disponível no site electrosome.com)
{
  char i;
  Lcd_Cmd(0x04);   // Set CGRAM Address
  Lcd_Cmd(0x00);   // .. set CGRAM Address
  for (i = 0; i <= 63 ; i++)
    Lcd_Write_Char(MyChar5x8[i]);
  Lcd_Cmd(0);      // Return to Home
  Lcd_Cmd(2);      // .. return to Home
}
//Fim da criação de caractere


void Lcd_Clear_Line(unsigned char L) {
    Lcd_Set_Cursor(L,1);
    Lcd_Write_String("                    ");   
}

*/
