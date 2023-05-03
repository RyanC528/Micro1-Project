/**
 *******************************************************************
 *  Lesson 5 - "Variable Speed Rotate"
 *
 *  This lesson combines all of the previous lessons to produce a variable speed rotating
 *  LED display that is proportional to the ADC value. The ADC value and LED rotate
 *  speed are inversely proportional to each other.
 *
 *  Rotate the POT counterclockwise to see the LEDs shift faster.
 *
 *  PIC: 16F1829
 *  Compiler: XC8 v1.00
 *  IDE: MPLABX v1.10
 *
 *  Board: PICkit 3 Low Pin Count Demo Board
 *  Date: 6.1.2012
 *
 * *******************************************************************
 * See Low Pin Count Demo Board User's Guide for Lesson Information*
 * ******************************************************************
 */

#include <htc.h>                                        //PIC hardware mapping
#define _XTAL_FREQ 500000                               //Used by the XC8 delay_ms(x) macro

//config bits that are part-specific for the PIC16F1829
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & LVP_OFF);

    /* -------------------LATC-----------------
     * Bit#:  -7---6---5---4---3---2---1---0---
     * LED:   ---------------|DS4|DS3|DS2|DS1|-
     *-----------------------------------------
     */

unsigned char adc(void);                                //prototype

void main(void) {
    unsigned char delay;


    OSCCON = 0b00111000;                                //500KHz clock speed
    TRISC = 0;                                          //all LED pins are outputs
    LATC = 0;
    LATCbits.LATC3 = 1;                                 //start sequence with DS4 lit

                                                        //setup ADC
    TRISAbits.TRISA4 = 1;                               //Potentiamtor is connected to RA4...set as input
    ANSELAbits.ANSA4 = 1;                               //analog
    ADCON0 = 0b00001101;                                //select RA4 as source of ADC and enable the module (AN3)
    ADCON1 = 0b00010000;                                //left justified - FOSC/8 speed - Vref is Vdd


//setup switch
    TRISBbits.TRISB0 = 1;                               //set switch as input
    OPTION_REGbits.nWPUEN = 0;                          //enable pull-up on PORTB
    WPUBbits.WPUB0 = 1;                                 //enable pull-up on RB0

    while (1) {
        // check switch
        if (paused == 0 && !PORTBbits.RB0) {
            // switch is pressed, pause rotation
            paused = 1;
            continue;
        } else if (paused == 1 && PORTBbits.RB0) {
            // switch is released, resume rotation
            paused = 0;
        }

        // read ADC value
        unsigned char adc_value = adc();

        // calculate delay based on ADC value
        delay = (unsigned char) (adc_value / 4);

        if (delay == 0) {
        if (PORTAbits.RA4 < 510) { // if the ADC value is less than 510, rotate left
            if (LATCbits.LATC0 == 0) { // if the leftmost LED is already lit, wrap around to the rightmost LED
                LATC = 0b10000000;
            }
            else {
                LATC <<= 1; // shift to the left to light up the next LED
            }
        }
        else if (PORTAbits.RA4 > 514) { // if the ADC value is greater than 514, rotate right
            if (LATCbits.LATC3 == 0) { // if the rightmost LED is already lit, wrap around to the leftmost LED
                LATC = 0b00001000;
            }
            else {
                LATC >>= 1; // shift to the right to light up the next LED
            }
        }
        // wait for the switch to be released
        while (PORTAbits.RA5 == 0) continue;
        // delay for 100ms to debounce the switch
        __delay_ms(100);
}
}

unsigned char adc(void) {
    __delay_us(5);                                      //wait for ADC charging cap to settle
    GO = 1;
    while (GO) continue;                                //wait for conversion to be finished

    return ADRESH;                                      //grab the top 8 MSbs

}
