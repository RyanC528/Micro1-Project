/**
 *******************************************************************
 * Lesson 5 - "Variable Speed Rotate"
 *
 * This lesson combines all of the previous lessons to produce a variable speed rotating
 * LED display that is proportional to the ADC value. The ADC value and LED rotate
 * speed are inversely proportional to each other.
 *
 * Rotate the POT counterclockwise to see the LEDs shift faster.
 *
 * PIC: 18F14K22
 * Compiler: XC8 v1.00
 * IDE: MPLABX v1.10
 *
 * Board: PICkit 3 Low Pin Count Demo Board
 * Date: 6.1.2012
 *
 * *******************************************************************
 * See Low Pin Count Demo Board User's Guide for Lesson Information*
 * ******************************************************************
 */

#include <htc.h>                        //PIC hardware mapping
#define _XTAL_FREQ 500000               //Used by the compiler for the delay_ms(x) macro

//config bits that are part-specific for the PIC18F14K22
__CONFIG(1, FOSC_IRC & PLLEN_OFF & FCMEN_OFF);
__CONFIG(2, PWRTEN_OFF & BOREN_OFF & WDTEN_OFF);
__CONFIG(3, HFOFST_OFF & MCLRE_OFF);
__CONFIG(4, STVREN_ON & LVP_OFF & DEBUG_ON);
__CONFIG(5, CP0_OFF & CP1_OFF & CPB_OFF & CPD_OFF);
__CONFIG(6, WRT0_OFF & WRT1_OFF & WRTC_OFF & WRTB_OFF & WRTD_OFF);
__CONFIG(7, EBTR0_OFF & EBTR1_OFF & EBTRB_OFF);

unsigned char adc(void); //prototype

    /* -------------------LATC-----------------
     * Bit#:  -7---6---5---4---3---2---1---0---
     * LED:   ---------------|DS4|DS3|DS2|DS1|-
     *-----------------------------------------
     */

void main(void) {
    unsigned char delay;                 //8 bit variable to hold ADC value (top 8 bits...forget the rest)

    OSCCON = 0b00100010;                 //500KHz clock speed
    TRISC = 0;                           //all LED pins are outputs
    LATC = 0b0001000;                    //start with DS4 lit by setting bit 3 HIGH in LATC
    TRISAbits.TRISA4 = 1;
    ANSELbits.ANS3 = 1; //configure AN3 as analog input
    ADCON0 = 0b00001101; //select AN3 as source of ADC, and enable the module
    ADCON1 = 0b00010000; //configure left justified result, and select VSS and VDD as references
    ADCON2 = 0b00000100; //select FOSC/32 speed
    ADFM = 1; //configure the result to be read from ADRESH only
    TRISAbits.TRISA5 = 1; //configure RA5 as input for switch
    OPTION_REGbits.nWPUEN = 0; //enable internal pull-ups
    WPUAbits.WPUA5 = 1; //enable pull-up for RA5

   while (1) {
    if (!PORTAbits.RA5) { //if switch is pressed
        while (!PORTAbits.RA5) continue; //wait for switch release
        __delay_ms(100); //add debounce delay
        while (1) { //pause loop
            if (!PORTAbits.RA5) { //if switch is pressed again
                while (!PORTAbits.RA5) continue; //wait for switch release
                __delay_ms(100); //add debounce delay
                break; //break out of pause loop
            }
        }
    } else { //if switch is not pressed
        unsigned char delay = adc() >> 1; //read ADC value and divide by 2
        __delay_ms(5); //delay for at least 5ms
        while (delay-- != 0){ //for each value of the ADC value, delay 2ms
            __delay_ms(2);
        }
        if (PORTAbits.RA4) { //if potentiometer arrow points right
            LATC << = 1; //shift to the left by 1
            if (LATC == 0) //when the first LED is lit, restart the pattern
                LATCbits.LATC0 = 1;
        } else if (PORTAbits.RA3) { //if potentiometer arrow points left
            LATC >> = 1; //shift to the right by 1
            if (STATUSbits.C) //when the last LED is lit, restart the pattern
                LATCbits.LATC3 = 1;
        }
    }
}


unsigned char adc(void) {
    __delay_us(5);                      //wait for ADC charging cap to settle
    GO = 1;
    while (GO) continue;                //wait for conversion to be finished
    return ADRESH;
}


/* 
 * File:   problem1.c
 * Authors: Ryan Copeland
 *
 * Created on May 2, 2023, 5:25 PM
 */

/*#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv) {

    return (EXIT_SUCCESS);
}
*/
