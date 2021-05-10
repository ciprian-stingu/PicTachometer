/** 
 * 
 * PicTachometer
 * 
 * Copyright (C) 2021 Ciprian Stingu
 * ciprian dot stingu at gmail dot com
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111 USA
 * or see <http://www.gnu.org/licenses/>
 *
 * File:   main.c
 * Author: Ciprian Stingu
 *
 * Created on May 8, 2021, 5:42 PM
 */
//------------------------------------------------------------------------------
#include "main.h"
#include "uart.h"
//------------------------------------------------------------------------------

void main(void) 
{
    // Enable all unmasked global INT
    GIE = 1;
    // Enable Peripheral interrupt
    PEIE = 1;
    // disable comparators
    CM0 = 1;
    CM1 = 1;
    CM2 = 1;
    
    DisableUnusedPorts();
    UartInit();
    Timer1Init();
    EventInit();
    
    __delay_ms(500);
    
    UartWriteString("PicTachometer ready...\n");
  
    TMR1IE = 1; // start timer1
    
    while(1)
    {
        if(ShowEvents) 
        {
            char convBuffer[6];
            
            uint16_t currentEventsPerSecond = 0;
            for(uint8_t i = 0; i < MAX_EVENTS; i++) {
                currentEventsPerSecond += EventsPerSecond[i];
            }
            
            UartWriteString("Events per minute: ");
            UnsignedWordToAsciiInteger(currentEventsPerSecond, convBuffer);
            UartWriteString(convBuffer);
            UartWriteString("\n");
            ShowEvents = FALSE;
        }
        else {
            asm("nop");
        }
    }
}
//------------------------------------------------------------------------------

void __interrupt() ISR(void)
{
    //new event
    if(INTF)
    {
        CurrentEvents++;
        INTF = 0;
    }
    
    //will be triggered at each 200ms
    if(TMR1IF)
    {
        TMR1H = 0x0B; //default value
        TMR1L = 0xDC; //3036
        TMR1IF = 0;
        
        if(++PeriodsPerSecond >= 5) //1 second
        {
            if(EventsPerSecondIndex >= MAX_EVENTS) {
                EventsPerSecondIndex = 0;
            }
            //record the last second
            EventsPerSecond[EventsPerSecondIndex++] = CurrentEvents;
            CurrentEvents = 0;
            PeriodsPerSecond = 0;
            ShowEvents = TRUE;
        }        
    }
    
    if(RCIF)
    {
        if(OERR) // check for overrun error and reset
        {
            CREN = 0; 
            CREN = 1;
        }
        
        RCIF = 0;
    }
}

//------------------------------------------------------------------------------

void Timer1Init(void)
{
    TMR1ON = 1;   //Enables Timer1
    TMR1CS = 0;   //Internal clock
    nT1SYNC = 1;  //Do not synchronize external clock input
    T1OSCEN = 0;  //Oscillator is shut off
    T1CKPS1 = 1;  //1:8 Prescale value
    T1CKPS0 = 1;  //1:8 Prescale value

    TMR1H = 0x0B; //default value 3036
    TMR1L = 0xDC; //which will trigger the timer at every 200ms
    
    TMR1IF = 0;   //clear int flag
    TMR1IE = 0;   //stop timer1
}
//------------------------------------------------------------------------------

void DisableUnusedPorts(void)
{
    //set all unused pins as outputs
    TRISA0 = 0;
    TRISA1 = 0;
    TRISA2 = 0;
    TRISA3 = 0;
    TRISA4 = 0;
    //reset pin as input
    TRISA5 = 1;
    TRISB3 = 0;
    TRISB4 = 0;
    TRISB5 = 0;
    TRISB6 = 0;
    TRISB7 = 0;
}
//------------------------------------------------------------------------------

void EventInit(void)
{
    //INT as input
    TRISB0 = 1;
    CurrentEvents = 0;
    for(uint8_t i = 0; i < MAX_EVENTS; i++) {
        EventsPerSecond[i] = 0;
    }
    EventsPerSecondIndex = 0;
    PeriodsPerSecond = 0;
    //enable INT interrupt
    INTE = 1;
}
//------------------------------------------------------------------------------

void UnsignedWordToAsciiInteger(uint16_t value, char *buffer)
{
 	if(value)
 	{
        uint16_t divisor;
        uint8_t i, printed = FALSE;
        
 		for(i = 0, divisor = 10000; i < 5; i++)
 		{
 			uint16_t digit = value / divisor;
 			if(digit || printed)
 			{
 				*buffer++ = '0' + digit;
 				value -= digit * divisor;
 				printed = TRUE;
 			}
 			divisor /= 10;
 		}
 	}
 	else {
 		*buffer++ = '0';
 	}
 
 	*buffer = NULL_CHAR;
}
//------------------------------------------------------------------------------
