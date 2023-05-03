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

//config bits that are part-specific for the PIC16F1829
__CONFIG(FOSC_INTOSC & WDTE_OFF & PWRTE_OFF & MCLRE_OFF & CP_OFF & CPD_OFF & BOREN_ON & CLKOUTEN_OFF & IESO_OFF & FCMEN_OFF);
__CONFIG(WRT_OFF & PLLEN_OFF & STVREN_OFF & LVP_OFF);

    /* -------------------LATC-----------------
     * Bit#:  -7---6---5---4---3---2---1---0---
     * LED:   ---------------|DS4|DS3|DS2|DS1|-
     *-----------------------------------------
     */

void main(void) {
    int count = 0;              //declare variable to track # of switch presses
    TRISC = 0;                  //need testing on these two to see what they do I believe it just resets the LED's to off state
    LATC = 0;
    
    TRISAbits.TRISA2 = 1;       //from example: setup switch, switch as input
    ANSELAbits.ANSA2 = 0;       //digital switch
    
    while(1) {
    if (SWITCH == DOWN) {
        count += 1;                     //when switch is pressed increase counter by 1
       if(count >= 16) {
        count = 0;                      //resets counter to 0 as it exceeds LED binary limits
       }
    }
    }
        
 //create test cases to match count # with binary number & LED's
