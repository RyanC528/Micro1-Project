/* 
 * File:   problem1.c
 * Authors: Ryan Copeland, Sam Guaetta
 *
 * Created on May 2, 2023, 5:25 PM
 */


#define DOWN        0
#define UP          1
#define SWITCH      PORTAbits.RA2
#define LED         LATCbits.LATC0

#include <htc.h>
#include <stdio.h>
#define _XTAL_FREQ 500000                               //Used by the XC8 delay_ms(x) macro


//config bits that are part-specific for the PIC16F1829
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & LVP_OFF);

    /* -------------------LATC-----------------
     * Bit#:  -7---6---5---4---3---2---1---0---
     * LED:   ---------------|DS4|DS3|DS2|DS1|-
     *-----------------------------------------
     */

void main(void) {
    TRISC = 0;                  //need testing on these two to see what they do I believe it just resets the LED's to off state
    OSCCON = 0b00111000;                                //500KHz clock speed
    LATC = 0;
    
    TRISAbits.TRISA2 = 1;       //from example: setup switch, switch as input
    ANSELAbits.ANSA2 = 0;       //digital switch
    
    int count = 0;      //declare variable to track # of switch presses
    
    while (1) {
        if (SWITCH == DOWN) {
            __delay_ms(50);
            if (SWITCH == UP){
                count += 1;                     //when switch is pressed increase counter by 1
                    if(count >= 16) {
                    count = 0;                      //resets counter to 0 as it exceeds LED binary limits
                    }
            }
        }
        if (count == 0) {                 // test cases to match count # with binary number & light up LED's accordingly
            LATC = 0b0000000; //0000
            }
        if (count == 1) {
            LATC = 0b0000001; //0001
            }
        if (count == 2) {
           LATC = 0b0000010; //0010
            }
        if (count == 3) {
            LATC = 0b0000011; //0011
            }
        if (count == 4) {
            LATC = 0b0000100; //0100
            }
        if (count == 5) {
            LATC = 0b0000101; //0101
            }
        if (count == 6) {
            LATC = 0b0000110; //0110
            }
        if (count == 7) {
            LATC = 0b0000111; //0111
            }
        if (count == 8) {
            LATC = 0b0001000; //1000
            }
        if (count == 9) {
            LATC = 0b0001001; //01001
            }
        if (count == 10) {
            LATC = 0b0001010; //1010
            }
        if (count == 11) {
            LATC = 0b0001011; //1011
            }
        if (count == 12) {
            LATC = 0b0001100; //1100
            }
        if (count == 13) {
            LATC = 0b0001101; //1101
            }
        if (count == 14) {
            LATC = 0b0001110; //1110
            }
        if (count == 15) {
            LATC = 0b0001111; //1111
            }
        if(count >= 16) {
            count = 0;                     //resets counter to 0 as it exceeds LED binary limits
            LATC = 0b0000000; //0000
            }
    }
}
