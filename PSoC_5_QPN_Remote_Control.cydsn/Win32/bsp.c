/*****************************************************************************
* Product: BSP for Remote Control, Win32
*
*                    Q u a n t u m     L e a P s
*                    ---------------------------
*                    innovating embedded systems
*
* Copyright (C) Quantum Leaps, LLC. All rights reserved.
*
* This program is open source software: you can redistribute it and/or
* modify it under the terms of the GNU General Public License as published
* by the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Alternatively, this program may be distributed and modified under the
* terms of Quantum Leaps commercial licenses, which expressly supersede
* the GNU General Public License and are specifically designed for
* licensees interested in retaining the proprietary status of their code.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*
* Contact information:
* Web:   www.state-machine.com
* Email: info@state-machine.com
*****************************************************************************/
/*****************************************************************************
* Company : Yantronix
* Author  : Satish Nair
* Mailto  : satishgn77@gmail.com
*****************************************************************************/

#include "qpn.h"
#include "bsp.h"
#include "remotecontrol.h"

#include <conio.h>
#include <stdlib.h>
#include <stdio.h>

Q_DEFINE_THIS_FILE

static int16_t J1_VRx_Result = 0, J1_VRy_Result = 0;
static uint16_t PWM_Compare = 0, PWM_Compare_Prev = 0;
	
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min, int32_t out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

/*..........................................................................*/
void BSP_init() {
    printf("Win32 Test\nQP-nano version: %s\n"
           "Press ESC to quit...\n",
           QP_VERSION_STR);
}

void BSP_printString(const char_t *string)
{
    printf(string);
}

void BSP_ledOff() {
    //printf("OFF\n");
}

void BSP_ledOn() {
    //printf("ON\n");
}

void BSP_ledFlash(void) {
    printf("LED Flashing...\n");
}

void BSP_setLedPWM(uint16_t PWM_Compare) {
    printf("LED Pulsing [%d]...\n", PWM_Compare);
}

uint32_t BSP_isADCEOC(void) {
    return true;
}

int16_t BSP_getADCResult(uint16_t adcChannel) {
	if (adcChannel == ADC_CHANNEL_NUMBER_0) {
		return J1_VRx_Result;
	} else if (adcChannel == ADC_CHANNEL_NUMBER_1) {
		return J1_VRy_Result;
	}
	return 0;
}

void BSP_sendCANMsgJ1VRxData(int16_t J1_VRx_Mid, int16_t J1_VRx_Max, int16_t J1_VRx_Out)
{	
	if (J1_VRx_Out < J1_VRx_Mid)
	{
		PWM_Compare = map(J1_VRx_Out, J1_VRx_Mid, 0, 0, 24000lu);
	}
	else
	{
		PWM_Compare = map(J1_VRx_Out, J1_VRx_Mid, J1_VRx_Max, 0, 24000lu);
	}
}

void BSP_sendCANMsgJ1VRyData(int16_t J1_VRy_Mid, int16_t J1_VRy_Max, int16_t J1_VRy_Out)
{
	(void)J1_VRy_Mid;
	(void)J1_VRy_Max;
	(void)J1_VRy_Out;	
}

void BSP_sendCANMsgJ1SWData(uint8_t switchState)
{
	(void)switchState;	
}

void Q_onAssert(char_t const Q_ROM * const file, int_t line) {
    fprintf(stderr, "\nAssertion failed in %s, line %d\n", file, line);
    exit(-1);
}

void QF_onStartup(void) {
    QF_setTickRate(BSP_TICKS_PER_SEC);
}

void QF_onCleanup(void) {
    printf("\nBye! Bye!\n");
}

void QF_onClockTickISR(void) {
    QF_tickXISR(0U); /* perform the QF-nano clock tick processing */

    if (_kbhit()) { /* any key pressed? */
        int ch = _getch();
        switch (ch)
		{
			case 27 : { /* ESC key pressed */
            QF_stop();
			break;
			}
			
			case 32 : { /* SHIFT key pressed */
			QACTIVE_POST_ISR(&AO_RemoteControl, SW_CHANGE_SIG, SWITCH_PRESSED);
			break;
			}

			case 72 : { /* UP key pressed */
            ++J1_VRy_Result;
			break;
			}

			case 80 : { /* DOWN key pressed */
            --J1_VRy_Result;
			break;
			}

			case 77 : { /* RIGHT key pressed */
            ++J1_VRx_Result;
			break;
			}

			case 75 : { /* LEFT key pressed */
            --J1_VRx_Result;
			break;
			}	
		}
    }
	
	if (PWM_Compare_Prev != PWM_Compare)
	{
		QACTIVE_POST_ISR(&AO_RemoteControl, CAN_RECEIVE_SIG, PWM_Compare);
		PWM_Compare_Prev = PWM_Compare;
	}
}




