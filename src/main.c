/***********************************************************************
 * 
 * Use Analog-to-digital conversion to read push buttons on LCD keypad
 * shield and display it on LCD screen.
 * 
 * ATmega328P (Arduino Uno), 16 MHz, PlatformIO
 *
 * Copyright (c) 2018 Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/


/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <gpio.h>           // GPIO library for AVR-GCC
#include <uart.h>           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <lcd.h>            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for number conversions

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and start ADC conversion every 100 ms.
 *           When AD conversion ends, send converted value to LCD screen.
 * Returns:  none
 **********************************************************************/
int main(void){

    uart_init(UART_BAUD_SELECT(9600,F_CPU));
    GPIO_mode_output(&DDRD,PD6);
    GPIO_mode_output(&DDRB,PB3);
    TCCR0A |= ((1UL<<7) | (1UL<<0) | (1UL<<1));
    //prescale clock with N=256
    TCCR0B |= (1UL<<2);
    TCCR2A |= ((1UL<<7) | (1UL<<0) | (1UL<<1));
    //prescale clock with N=256
    TCCR2B |= ((1UL<<2) | (1UL<<1));

    // Configure Analog-to-Digital Convertion unit
    // Select ADC voltage reference to "AVcc with external capacitor at AREF pin"
    ADMUX |= (1UL<<6);
    // Select input channel ADC0 (voltage divider pin)
    ADMUX &= ~((1UL<<0)|(1UL<<1)|(1UL<<2)|(1UL<<3));
    // Enable ADC module
    ADCSRA |= (1UL<<7);
    // Enable conversion complete interrupt
    ADCSRA |= (1UL<<3);
    // Set clock prescaler to 128
    ADCSRA |= (1UL<<0)|(1UL<<1)|(1UL<<2);

    lcd_init(LCD_DISP_ON);
    lcd_gotoxy(1, 0); lcd_puts("x");
    lcd_gotoxy(1, 1); lcd_puts("y");


    TIM1_overflow_4ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    OCR0A=50;
    OCR2A=50;

    //OCR0A = 0;

   

    // Infinite loop
    while (1){
      
    }

    // Will never reach this
    return 0;
}


/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Use single conversion mode and start conversion every 100 ms.
 **********************************************************************/
ISR(TIMER1_OVF_vect){
  static uint8_t overfl=0;
  static uint16_t val1=0, val2=0;
  uint16_t cele, desetiny;
  char str[4];

  overfl++;
  if(overfl>=40){
    // Start ADC conversion
    ADMUX &= ~((1UL<<0)|(1UL<<1)|(1UL<<2)|(1UL<<3));
    ADCSRA |= (1UL<<6);

    val1 = ADC;

    ADMUX |= (1UL<<0);
    do{
        ;
    }while((ADCSRA&16)==0);
    ADCSRA |= (1UL<<6);
    
    val2 = ADC;
    
    OCR0A=val1*(115.0/1024.0)+35;
    OCR2A=val2*(115.0/1024.0)+35;

    sprintf(str,"%3d",(uint16_t)((float)val1*(180.0/1024.0)));
    lcd_gotoxy(3, 0);
    lcd_puts(str);
    lcd_putc(223);

    sprintf(str,"%3d",(uint16_t)((float)val2*(180.0/1024.0)));
    lcd_gotoxy(3, 1);
    lcd_puts(str);
    lcd_putc(223);
    overfl=0;

    if(val1>400&&val1<600){
      lcd_gotoxy(13, 0);
      lcd_puts(" | ");
    }
    else if(val1<=400){
      lcd_gotoxy(13, 0);
      lcd_puts("<  ");
    }
    else if(val1>=600){
      lcd_gotoxy(13, 0);
      lcd_puts("  >");
    }
    if(val2>400&&val2<600){
      lcd_gotoxy(13, 1);
      lcd_puts(" - ");
    }
    else if(val2<=400){
      lcd_gotoxy(13, 1);
      lcd_putc(94);
      lcd_puts("  ");
    }
    else if(val2>=600){
      lcd_gotoxy(13, 1);
      lcd_puts("  v");
    }
  }
  else if(overfl>=40){
      overfl=0;
  }
}

/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display converted value on LCD screen.
 **********************************************************************/
ISR(ADC_vect){
    ;
}

/*https://docs.arduino.cc/retired/boards/arduino-uno-rev3-with-long-pins
https://www.teachmemicro.com/arduino-servo-motor-tutorial/
https://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-7810-Automotive-Microcontrollers-ATmega328P_Datasheet.pdf*/