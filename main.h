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
 * File:   main.h
 * Author: Ciprian Stingu
 *
 * Created on May 8, 2021, 5:42 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#include <xc.h>
//#include <pic16f628a.h> 
#include <stdint.h>

#ifdef	__cplusplus
extern "C" {
#endif

#pragma config CP    = OFF
#pragma config CPD   = OFF
#pragma config LVP   = OFF
#pragma config BOREN = ON
#pragma config MCLRE = OFF
#pragma config PWRTE = OFF
#pragma config WDTE  = OFF
#pragma config FOSC  = HS

#define _XTAL_FREQ 10000000

#define NULL_CHAR 0x00
#define FALSE 0
#define TRUE 1

//60 seconds buffer
#define MAX_EVENTS 60
//no of events in the last second
uint8_t CurrentEvents = 0;
//keep the last 60s events
uint8_t EventsPerSecond[MAX_EVENTS];
//which item will be written
uint8_t EventsPerSecondIndex = 0;
//at every 5 periods of 200ms, record the no of events
uint8_t PeriodsPerSecond = 0;
//at every second display the no o events for the last minute
uint8_t ShowEvents = TRUE;

void __interrupt() ISR(void);
void Timer1Init(void);
void EventInit(void);
void DisableUnusedPorts(void);
void UnsignedWordToAsciiInteger(uint16_t value, char *buffer);

#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

