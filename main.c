/* 
 * File:   main.c
 * Author: Daniel Bradley
 *
 * Created on April 2, 2018, 3:00 PM
 */

#include <p18f8722.h>
#include "Messages.h"
#include "PIC18LCDDisplayDriver.h"
#include <delays.h>
#include <stdio.h>
#include <stdlib.h>

#pragma   config	OSC = HSPLL		//	Oscillator selection: HS with PLL enabled
#pragma   config	FCMEN = OFF		// 	Fail-safe clock monitor disabled
#pragma   config	IESO = OFF		//	Internal/External Oscillator switchover disabled
#pragma   config	PWRT = OFF     	//  Power-up Timer disabled    
#pragma   config	BOREN = OFF		//	Brown out reset disabled
#pragma   config	WDT = OFF 		//  Watchdog timer Off 
#pragma   config	MCLRE = ON  	//  MCLR pin enabled, RE3 input pin disabled
#pragma   config	LVP = OFF		//  Single supply ICSP disabled  
#pragma   config	XINST = OFF 	// 	Instruction set extension and indexed addressing mode disabled (legacy mode)

#define LED0 PORTDbits.RD0
#define LED1 PORTDbits.RD1
#define LED2 PORTDbits.RD2
#define LED3 PORTDbits.RD3
#define LED4 PORTDbits.RD4
#define LED5 PORTDbits.RD5
#define LED6 PORTDbits.RD6
#define LED7 PORTDbits.RD7

int scoreForOne = 0;
int scoreForTwo = 0;
int speedValue = 255;
int flashCount;
int loop = 0;
int starting;
unsigned char d = 200;	// delay between character display
int B0Button = 0;
int A5Button = 0;

void convertScores(void){                   // Define function to convert scores from type INT to individual char's in array scores
    int t = 3;                              // Array index value for Player 2
    int i = 14;                             // Array index value for Player 1
    if (scoreForOne == 0){                  // If score for player 1 is zero, print zero to LCD
        scores[13] = '0';
    }else{                                  
        while(scoreForOne){                 // While scoreForOne has a non-zero value loop find it's modulus, convert to ASCII and store in the scores array at index i where i starts at 3 and decreases by 1 for each number
            scores[i]=(scoreForOne%10)+'0';
            scoreForOne= scoreForOne/10;
            i--;
        }
    }
    if(scoreForTwo == 0){                    // If score for player 2 is zero, print zero to LCD
        scores[2] = '0';
    }
    else{
        while(scoreForTwo){
            scores[t]=(scoreForTwo%10)+'0';
            scoreForTwo = scoreForTwo/10;
            t--;
        }
    }
}

void EndGameInterrupt(void);      // Function prototype for Game interrupt

void main(void){

	// Set up ports and bits
    LCDInit();              // Run initialization of LCD screen
	LCDClear();             // Clear the entire LCD screen
    TRISAbits.RA5 = 1;      // Set up PORTA, A5 as input
	TRISBbits.RB0 = 1;		// Set up PORTB, B0 as input
    TRISBbits.RB2 = 1;      // Set up PORTB, B2 as input
	TRISD = 0;              // Set up all PORTD as output
    PORTD = 0x00;           // Turn off all LED's 
    starting = 1;           // Set up variable 'starting' so that game can begin
    T0CON = 0b00000011;     // Timer0, 16-bit mode, 1:16 prescale
    

	// Set up interrupts
	INTCONbits.INT0IF = 0;		// Clear RB0 interrupt flag
	INTCONbits.INT0IE = 1;      // Enable RB0 as interrupt source
    INTCONbits.TMR0IF = 0;      // Clear TMR0 interrupt flag
    INTCONbits.TMR0IE = 1;      // Enable TMR0 as interrupt source
    INTCON3bits.INT2IF= 0;      // Clear RB2 as interrupt source
    INTCON3bits.INT2IE= 1;      // Enable RB2 as interrupt source
	INTCON2bits.INTEDG0 = 0;	// Make RB0 negative edge - remember button press high to low
    INTCON2bits.INTEDG2 = 0;    // Make RB1 negative edge - remember button press high to low
	INTCONbits.GIE = 1;     	// Enable all interrupts globally
    
    while (1){                      // Loop to check if B0Button or A5Button are equal to one
        if (B0Button == 1){         // If B0Button is equal to one due to interrupt being activated run below code
            T0CONbits.TMR0ON = 0;   // Stop the TMR0 when the button is pressed 
            TMR0H = 0x48;           // Reload timer value for first part of a 16bit timer
            TMR0L = 0xE5;           // Reload timer value for second part of a 16bit timer
            scoreForTwo = scoreForTwo+1;         // Increase score value for player two by one
            PORTD = 0x00;                        // Turn off all LED's on PORTD
            LED7 = 1;                            // Turn on the LED closest to button B0
            Delay10KTCYx(speedValue);            // Delay between LED bitshift's (Delay function starts being passed 250 and decreases by 5 with each rebound of the 'Ball')
            Delay10KTCYx(speedValue);
            starting = 0;                        // Since game has started, starting variable is set to 0;
            while (LED0 != 1){                   // While the lit LED is not the last LED in the strip opposite to the button pushed (RB0), run the below code
                PORTD = PORTD >> 1;              // Bitshift PORTD to the right by 1
                Delay10KTCYx(speedValue);        // Delay between LED bitshift's (Delay function starts being passed 250 and decreases by 5 with each rebound of the 'Ball')
                Delay10KTCYx(speedValue);
            }
            B0Button = 0;                        // Reset value of B0Button to zero so code block is exited loop continues to check variable values
            T0CONbits.TMR0ON = 1;                // Start TMR0 to count down to end game if A5Button is not pushed
        }
        if (A5Button == 1){                      // If A5Button is equal to one due to interrupt being activated run below code
            T0CONbits.TMR0ON = 0;                // Stop the TMR0 when the button is pressed
            TMR0H = 0x48;                        // Reload timer value for first part of a 16bit timer
            TMR0L = 0xE5;                        // Reload timer value for second part of a 16bit timer
            scoreForOne = scoreForOne+1;         // Increase score value for player one by on          
            PORTD = 0x00;                        // Turn off all LED's on PORTD
            LED0 = 1;                            // Turn on the LED closest to button A5
            Delay10KTCYx(speedValue);            // Delay between LED bitshift's (Delay function starts being passed 250 and decreases by 5 with each rebound of the 'Ball')
            Delay10KTCYx(speedValue);
            starting = 0;                        // Since game has started, starting variable is set to 0;
            while (LED7 != 1){                   // While the lit LED is not the last LED in the strip opposite to the button pushed (RA5), run the below code
                PORTD = PORTD << 1;              // Bitshift PORTD to the left by 1
                Delay10KTCYx(speedValue);        // Delay between LED bitshift's (Delay function starts being passed 250 and decreases by 5 with each rebound of the 'Ball')
                Delay10KTCYx(speedValue);
            }
            A5Button = 0;                        // Reset value of A5Button to zero so code block is exited loop continues to check variable values
            T0CONbits.TMR0ON = 1;                // Start TMR0 to count down to end game if B0Button is not pushed
        }
    }
}

#pragma code My_HighPriority_Interrupt=0x08      // Direct the high priority interrupt at memory location 0x08 to the function "EndGameInterrupt"
void My_HighPriority_Interrupt(void){
	
	_asm
		GOTO EndGameInterrupt
	_endasm

}

#pragma interrupt EndGameInterrupt
void EndGameInterrupt(void){                     // Interrupt function
    
    if((INTCON3bits.INT2IF == 1 && LED0 != 1 && starting == 0)||(INTCONbits.TMR0IF == 1 && LED0 == 1)){     // If the RA5 is pressed and LED0 is not on and it's not the start of the game or RB0 is pushed and LED0 is on, run the code block below
        INTCONbits.TMR0IF = 0;                   // Set the TMR0 interrupt flag to 0
        T0CONbits.TMR0ON = 0;                    // Turn off the timer TMR0
        TMR0H = 0x48;                            // Reload timer value for first part of a 16bit timer
        TMR0L = 0xE5;                            // Reload timer value for second part of a 16bit timer
        INTCON3bits.INT2IF = 0;                  // Set the INT2 interrupt flag to 0
        INTCONbits.INT0IF = 0;                   // Set the TMR0 interrupt flag to 0
        PORTD = 0x00;                            // Turn off all LED's attached to PORTD
        for(flashCount = 0; flashCount <= 9; flashCount = flashCount++){                                    // Loop the complement of LED7's state 9 times to make it flash on and off 5 times
            LED7 = ~LED7;
            Delay10KTCYx(250); 
        }
        PORTD = 0xFF;                            // Turn all LED's on PORTD on
        LCDLine_1();                             // Write to line 1 of the display 
        d_write_line_delay(lineClear, 20);       // Clear line 2 with a delay of 20 between the characters
        LCDLine_1();                             // Write to line 1 of the display 
        d_write_line_delay(playerTitle, d);      // Write the text "Player2 Player1" to line one of the LCD with a delay of 200 between the characters
        convertScores();                         // Call the function convertScores to write the scores for each player to the variable scores
        LCDLine_2();                             // Write to line 2 of the display
        d_write_line_delay(scores, 0);           // Write the value of the variable "scores" to line two of the LCD with a delay of 0 between the characters
        Delay10KTCYx(250);                       // Maximum value of delay function called 4 times to allow score to remain on the LCD
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        LCDLine_2();                             // Write to line 2 of the display
        d_write_line_delay(lineClear, 0);        // Clear line 2 with a delay of 0 between the characters
        LCDLine_2();                             // Write to line 2 of the display
        d_write_line_delay(winnerTwo, d);        // Write the word "Winner" to line 2 under Player 2 with a delay of 200 between each character
        Delay10KTCYx(250);                       // Maximum value of delay function called 4 times to allow the word "Winner" to remain on the LCD
         Delay10KTCYx(250);
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        LCDLine_2();                             // Write to line 2 of the display
        d_write_line_delay(lineClear, 0);        // Clear line 2 with a delay of 0 between the characters
        LCDLine_1();                             // Write to line 1 of the display
        d_write_line_delay(lineClear, 0);        // Clear line 1 with a delay of 0 between the characters
        PORTD = 0x00;                            // Turn off all LED's attached to PORTD
        starting = 1;                            // Reset value of starting variable for next game
        B0Button = 0;                            // Reset value of B0Button variable for next game
        A5Button = 0;                            // Reset value of A5Button variable for next game
        speedValue = 255;                        // Reset value of speedValue variable for next game
        scores[13] = ' ';
        scores[12] = ' ';
        scores[11] = ' ';
        scores[2] = ' ';
        scores[1] = ' ';
        scores[0] = ' ';
        main();                                  // Call main function to restart check for button state variable     
    }else if((INTCONbits.INT0IF == 1 && LED7 != 1  && starting == 0)||(INTCONbits.TMR0IF == 1 && LED7 == 1)){    // If the RB0 is pressed and LED7 is not on and it's not the start of the game or RA5 is pushed and LED7 is on, run the code block below  
        INTCONbits.TMR0IF = 0;                   // Set the TMR0 interrupt flag to 0
        T0CONbits.TMR0ON = 0;                    // Turn off the timer TMR0
        TMR0H = 0x48;                            // Reload timer value for first part of a 16bit timer
        TMR0L = 0xE5;                            // Reload timer value for second part of a 16bit timer
        INTCON3bits.INT2IF = 0;                  // Set the INT2 interrupt flag to 0
        INTCONbits.INT0IF = 0;                   // Set the INT0 interrupt flag to 0
        PORTD = 0x00;                            // Turn off all LED's attached to PORTD
        for(flashCount = 0; flashCount <= 9; flashCount = flashCount++){                                         // Loop the complement of LED7's state 9 times to make it flash on and off 5 times
            LED0 = ~LED0;
            Delay10KTCYx(250); 
        }
        PORTD = 0xFF;                            // Turn all LED's on PORTD on
        LCDLine_1();                             // Write to line 1 of the display 
        d_write_line_delay(lineClear, 20);       // Clear line 2 with a delay of 20 between the characters
        LCDLine_1();                             // Write to line 1 of the display 
        d_write_line_delay(playerTitle, d);      // Write the text "Player2 Player1" to line one of the LCD with a delay of 200 between the characters
        convertScores();                         // Call the function convertScores to write the scores for each player to the variable scores
        LCDLine_2();                             // Write to line 2 of the display
        d_write_line_delay(scores, 0);           // Write the value of the variable "scores" to line two of the LCD with a delay of 0 between the characters
        Delay10KTCYx(250);                       // Maximum value of delay function called 4 times to allow score to remain on the LCD
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        LCDLine_2();                             // Write to line 2 of the display
        d_write_line_delay(lineClear, 0);        // Clear line 2 with a delay of 0 between the characters
        LCDLine_2();                             // Write to line 2 of the display
        d_write_line_delay(winnerOne, d);        // Write the word "Winner" to line 2 under Player 1 with a delay of 200 between each character
        Delay10KTCYx(250);                       // Maximum value of delay function called 4 times to allow the word "Winner" to remain on the LCD
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        Delay10KTCYx(250);
        LCDLine_1();                             // Write to line 1 of the display
        d_write_line_delay(lineClear, 0);        // Clear line 1 with a delay of 0 between the characters
        LCDLine_2();                             // Write to line 1 of the display
        d_write_line_delay(lineClear, 0);        // Clear line 2 with a delay of 0 between the characters
        PORTD = 0x00;                            // Turn off all LED's attached to PORTD
        starting = 1;                            // Reset value of starting variable for next game
        B0Button = 0;                            // Reset value of B0Button variable for next game
        A5Button = 0;                            // Reset value of A5Button variable for next game
        speedValue = 255;                        // Reset value of speedValue variable for next game
        scores[13] = ' ';
        scores[12] = ' ';
        scores[11] = ' ';
        scores[2] = ' ';
        scores[1] = ' ';
        scores[0] = ' ';   
        main();                                  // Call main function to restart check for button state variable     
    }else if(INTCONbits.INT0IF == 1 && (LED7 == 1 || starting == 1)){           // If interrupt is activated when LED7 or starting variable is 1, run code block
        INTCON3bits.INT2IF = 0;                  // Set the INT2 interrupt flag to 0
        INTCONbits.INT0IF = 0;                   // Set the INT0 interrupt flag to 0
        if (speedValue != 5){
            speedValue = speedValue - 5;             // Decrease the speedValue variable by 5;
        }
        B0Button = 1;                            // Set B0Button variable to 1;
    }else if(INTCON3bits.INT2IF == 1 && (LED0 == 1 || starting == 1)){          // If interrupt is activated when LED0 or starting variable is 1, run code block
        INTCON3bits.INT2IF = 0;                  // Set the INT2 interrupt flag to 0
        INTCONbits.INT0IF = 0;                   // Set the INT0 interrupt flag to 0
        if (speedValue != 5){
            speedValue = speedValue - 5;         // Decrease the speedValue variable by 5;
        }
        A5Button = 1;                            // Set B0Button variable to 1;
    }
}